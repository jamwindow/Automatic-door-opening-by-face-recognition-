import paho.mqtt.client as mqtt
from PIL import Image, ImageOps
import numpy as np
from Adafruit_IO import MQTTClient  # Import Adafruit IO MQTT library
from keras.models import load_model
import time
import base64

# Adafruit IO Credentials
AIO_USERNAME = ""  # Replace with your Adafruit IO username
AIO_KEY = ""  # Replace with your Adafruit IO Key
AIO_FEED_NAME = "project-242.name"  # Replace with your feed name
AIO_FEED_IMAGE = "picture"
AIO_FEED_DOOR = "project-242.door"

door_state = "OFF"

# Initialize MQTT Client
aio_client  = MQTTClient(AIO_USERNAME, AIO_KEY)

# Load the model
model = load_model("keras_Model.h5", compile=False)

# Load labels
class_names = open("labels.txt", "r").readlines()

# MQTT Callback Functions
def connected(client):
    print("Connected to Adafruit IO!")
    client.subscribe(AIO_FEED_IMAGE)

def disconnected(client):
    print("Disconnected from Adafruit IO!")


def image_detector():
    global door_state
    data = np.ndarray(shape=(1, 224, 224, 3), dtype=np.float32)

    # Open and process the image
    image = Image.open("received.jpg").convert("RGB")
    image = ImageOps.fit(image, (224, 224), Image.Resampling.LANCZOS)
    image_array = np.asarray(image)
    normalized_image_array = (image_array.astype(np.float32) / 127.5) - 1
    data[0] = normalized_image_array

    # Run AI model inference
    prediction = model.predict(data)
    index = np.argmax(prediction)
    confidence_score = prediction[0][index]

    # Get class name
    ai_result = class_names[index]
    print("AI Result:", ai_result)
    print("Confidence Score:", confidence_score)

    if ai_result.strip() != "Unknow":
        door_state = "ON"
    else :
        door_state = "OFF"

    try:
        aio_client.publish(AIO_FEED_DOOR, door_state)
        aio_client.publish(AIO_FEED_NAME, ai_result)
        print("Data sent to Adafruit IO:", ai_result)
    except Exception as e:
        print("Failed to send data to Adafruit IO:", e)



# Callback when new data is received
def message(client, feed_id, payload):
    global door_state
    print("Image received from Adafruit IO!")
    try:
        # Decode Base64 data
        image_data = base64.b64decode(payload)

        # Save the image
        with open("received.jpg", "wb") as img_file:
            img_file.write(image_data)

        print("Image saved as received.jpg")

        image_detector()

        if door_state == "ON":
            time.sleep(10)


    except Exception as e:
        print("Error processing image:", e)


aio_client.on_connect = connected
aio_client.on_message = message
aio_client.on_disconnect = disconnected

aio_client.connect()
aio_client.loop_blocking()  # Keep MQTT connection alive in the background
############################################################################



