import boto3
import io
from PIL import Image, ImageDraw, ImageFont

def show_text(photo, bucket):

    session = boto3.Session(profile_name='default')
    client = session.client('rekognition')

    # Load image from S3 bucket
    s3_connection = boto3.resource('s3')
    s3_object = s3_connection.Object(bucket, photo)
    s3_response = s3_object.get()

    stream = io.BytesIO(s3_response['Body'].read())
    image = Image.open(stream)

    # Call detect text
    response = client.detect_text(Image={'S3Object': {'Bucket': bucket, 'Name': photo}})
    textDetections = response['TextDetections']



    imgWidth, imgHeight = image.size
    draw = ImageDraw.Draw(image)

    # calculate and display bounding boxes
    print('Detected text for ' + photo)
    for text in textDetections:
        print('Detected text:' + text['DetectedText'])
        print('Confidence: ' + "{:.2f}".format(text['Confidence']) + "%")
        print('Id: {}'.format(text['Id']))
        if 'ParentId' in text:
            print('Parent Id: {}'.format(text['ParentId']))
        print('Type:' + text['Type'])
        print('Bounding Box: ')
        print('   Width: ' + '{:.2f}'.format(text['Geometry']['BoundingBox']['Width']))
        print('   Height: ' + '{:.2f}'.format(text['Geometry']['BoundingBox']['Height']))
        print('   Left: ' + '{:.2f}'.format(text['Geometry']['BoundingBox']['Left']))
        print('   Top: ' + '{:.2f}'.format(text['Geometry']['BoundingBox']['Top']))
        
        print()

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
        try:
            draw.text((left+2, top+2), text['DetectedText'], fill="black")
        except:
            print("Couldn't draw")

        # Alternatively can draw rectangle. However you can't set line width.
        # draw.rectangle([left,top, left + width, top + height], outline='#00d400')

    image.show()

    return len(textDetections)

def main():
    bucket = 'water-meter-images-test'
    photo = 'digits2.png'
    faces_count = show_text(photo, bucket)
    print("text detected: " + str(faces_count))

if __name__ == "__main__":
    main()