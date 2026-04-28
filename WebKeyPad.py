from flask import Flask, Response
import mediapipe as mp
import cv2 as cv
import threading
import time
import requests  # ADD THIS for HTTP requests

app = Flask(__name__)

# ADD ESP32 configuration
ESP32_IP = "192.168.1.6"  # Replace with your ESP32's IP address
ESP32_URL = f"http://{ESP32_IP}/unlockDoor"  # You'll create this endpoint

mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

video = cv.VideoCapture(1)  # your phone camera



buttons = [
    {"label": "1", "pos": (350, 50),  "size": (40, 40)},
    {"label": "2", "pos": (400, 50),  "size": (40, 40)},
    {"label": "3", "pos": (450, 50),  "size": (40, 40)},
    {"label": "4", "pos": (350, 100), "size": (40, 40)},
    {"label": "5", "pos": (400, 100), "size": (40, 40)},
    {"label": "6", "pos": (450, 100), "size": (40, 40)},
    {"label": "7", "pos": (350, 150), "size": (40, 40)},
    {"label": "8", "pos": (400, 150), "size": (40, 40)},
    {"label": "9", "pos": (450, 150), "size": (40, 40)},
    {"label": "*", "pos": (350, 200), "size": (40, 40)},
    {"label": "0", "pos": (400, 200), "size": (40, 40)},
    {"label": "#", "pos": (450, 200), "size": (40, 40)},
]

PassWord = ['1', '#', '9']
UserPass = []

output_frame = None

# cooldown system (no lag)
last_press_time = 0
cooldown = 0.4  # 400 ms

# ADD THIS FUNCTION to send signal to ESP32
def send_to_esp32(action="success"):
    """Send HTTP request to ESP32 when password is correct"""
    try:
        # Option 1: Send to a specific endpoint
        response = requests.get(f"http://{ESP32_IP}/password_success", timeout=1)
        
        if response.status_code == 200:
            print(f" Signal sent to ESP32: {response.text}")
        else:
            print(f"❌ ESP32 returned: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f" Could not reach ESP32: {e}")

def camera_loop():
    global output_frame, UserPass, last_press_time

    with mp_hands.Hands(min_detection_confidence=0.8, min_tracking_confidence=0.5) as hands:
        while True:
            success, frame = video.read()
            if not success:
                continue
            
            

            image = cv.cvtColor(frame, cv.COLOR_BGR2RGB)
            image.flags.writeable = False
            results = hands.process(image)

            image.flags.writeable = True
            image = cv.cvtColor(image, cv.COLOR_RGB2BGR)

            if results.multi_hand_landmarks:
                left_hand_closed = False
                right_hand_landmarks = None

                # draw buttons
                for btn in buttons:
                    x, y = btn["pos"]
                    w, h = btn["size"]
                    cv.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), -1)
                    cv.putText(image, btn["label"], (x + 10, y + 30),
                               cv.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 3)

                # detect hands
                for num, hand in enumerate(results.multi_hand_landmarks):
                    mp_drawing.draw_landmarks(image, hand, mp_hands.HAND_CONNECTIONS)
                    handedness = results.multi_handedness[num].classification[0].label
                    landmarks = hand.landmark

                    count = 0
                    for t, p in zip([8, 12, 16, 20], [6, 10, 14, 18]):
                        if landmarks[t].y > landmarks[p].y:
                            count += 1
                    if landmarks[3].x > landmarks[4].x:
                        count += 1

                    if handedness == "Left" and count == 5:
                        left_hand_closed = True

                    if handedness == "Right":
                        right_hand_landmarks = landmarks
               
                if right_hand_landmarks is not None:
                    for btn in buttons:
                        frame_h, frame_w, _ = image.shape
                        finger_x = int(right_hand_landmarks[8].x * frame_w)
                        finger_y = int(right_hand_landmarks[8].y * frame_h)
                        x, y = btn["pos"]
                        w, h = btn["size"]
                        if x <= finger_x <= x + w and y <= finger_y <= y + h:
                            cv.rectangle(image, (x, y), (x + w, y + h), (0, 0, 255), -1)
                            cv.putText(image, btn["label"], (x + 10, y + 30),
                                        cv.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 3)


                            # button press logic (no lag)
                            if left_hand_closed:
                                frame_h, frame_w, _ = image.shape
                                finger_x = int(right_hand_landmarks[8].x * frame_w)
                                finger_y = int(right_hand_landmarks[8].y * frame_h)

                                for btn in buttons:
                                    x, y = btn["pos"]
                                    w, h = btn["size"]

                                    if x <= finger_x <= x + w and y <= finger_y <= y + h:
                                        current_time = time.time()
                            

                                        if current_time - last_press_time > cooldown:
                                            if len(UserPass) != len(PassWord):
                                                UserPass.append(btn["label"])
                                                print(btn["label"], UserPass)
                                                last_press_time = current_time
                
            
                # password check
                if UserPass == PassWord:
                    print(" PASSWORD CORRECT! Sending signal to ESP32...")
                    send_to_esp32()  # ADD THIS LINE
                    # Optional: Add visual feedback on screen
                    cv.putText(image, "ACCESS GRANTED", (50, 300), 
                              cv.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 4)
                    UserPass.clear()
                elif len(UserPass) == len(PassWord):
                    print(" PASSWORD ERROR!")
                    cv.putText(image, "ACCESS DENIED", (50, 300), 
                              cv.FONT_HERSHEY_SIMPLEX, 2, (0, 0, 255), 4)
                    UserPass.clear()

            # show on laptop
            cv.imshow("Camera", image)
            if cv.waitKey(1) & 0xFF == ord('q'):
                break

            # send to phone
            output_frame = image.copy()


def generate():
    global output_frame
    while True:
        if output_frame is None:
            continue

        _, buffer = cv.imencode('.jpg', output_frame)
        frame = buffer.tobytes()

        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')


@app.route('/')
def index():
    return """
    <html>
        <body style="margin:0">
            <img src="/video" width="100%">
        </body>
    </html>
    """


@app.route('/video')
def video_feed():
    return Response(generate(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')


# start camera thread
threading.Thread(target=camera_loop).start()

# start server
app.run(host='0.0.0.0', port=5000)