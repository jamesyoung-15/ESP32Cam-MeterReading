import json
import urllib.parse
import boto3
import io
from PIL import Image, ImageDraw, ImageFont

print('Loading function')

s3 = boto3.client('s3')
rekognition_client = boto3.client('rekognition')


def lambda_handler(event, context):
    print("Received event: " + json.dumps(event, indent=2))

    # Create a dictionary to store each digit detected information
    digit_info = {}

    # Get S3 Bucket image
    bucket = event['Records'][0]['s3']['bucket']['name']
    key = urllib.parse.unquote_plus(event['Records'][0]['s3']['object']['key'], encoding='utf-8')
    print("Image filename: " + key)

    # Get s3 Bucket image, and open image as pillow image object
    s3_object = s3.Object(bucket, key)
    s3_response = s3_object.get()
    stream = io.BytesIO(s3_response['Body'].read())
    image = Image.open(stream)
    # Setup canvas for displaying results
    imgWidth, imgHeight = image.size
    draw = ImageDraw.Draw(image)

    try:
        # Trigger Rekognition text detection
        response = rekognition_client.detect_text(Image={'S3Object': {'Bucket': bucket, 'Name': key}})
        textDetections = response['TextDetections']
        totalDetectedText = len(textDetections)
        print('Number of detected text: ' + textDetections)
        print('Detected text\n----------')
        # Go through each text detected
        for text in textDetections:
            # print info to debug
            print('Detected text:' + text['DetectedText'])
            print('Confidence: ' + "{:.2f}".format(text['Confidence']) + "%")
            print('Id: {}'.format(text['Id']))
            if 'ParentId' in text:
                print('Parent Id: {}'.format(text['ParentId']))
            print('Type:' + text['Type'])
            print()
            
            # for drawing bounding boxes to visualize result
            box = text['Geometry']['BoundingBox']
            left = imgWidth * box['Left']
            top = imgHeight * box['Top']
            width = imgWidth * box['Width']
            height = imgHeight * box['Height']
            points = (
                (left, top),
                (left + width, top),
                (left + width, top + height),
                (left, top + height),
                (left, top)

            )
            draw.line(points, fill='#00d400', width=2)
            # draw detected text to new image
            try:
                draw.text((left+2, top+2), text['DetectedText'], fill="yellow")
            except:
                print("Couldn't draw")

            # cleanup: replace spaces, replace non-digit text with 0s (eg. sometimes 0 is detected as U)
            digit = text['DetectedText'].replace(" ", "")
            digit = ''.join('0' if not char.isdigit() else char for char in digit)
            left_position = round(text['Geometry']['BoundingBox']['Left'],2)
            if not left_position in digit_info and int(digit)<10:
                digit_info[left_position] = digit
    
        number = ""
        for num in digit_info.values():
            number+=num
        
        print(number)

        img_bytes = BytesIO()
        imgage.save(img_bytes, format='JPEG')
        img_bytes = img_bytes.getvalue()
        filename = "result" + key
        s3.upload_fileobj(BytesIO(img_bytes), 'water-meter-rekognition-result', filename)

        return number
    
    except Exception as e:
        print(e)
        print('Error getting object {} from bucket {}. Make sure they exist and your bucket is in the same region as this function.'.format(key, bucket))
        raise e
              
