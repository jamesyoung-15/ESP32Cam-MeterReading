# Copyright 2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
# PDX-License-Identifier: MIT-0 (For details, see https://github.com/awsdocs/amazon-rekognition-developer-guide/blob/master/LICENSE-SAMPLECODE.)

import boto3

def detect_text(photo, bucket):

    session = boto3.Session(profile_name='default')
    client = session.client(service_name='rekognition')

    # client = boto3.client('rekognition')

    response = client.detect_text(Image={'S3Object': {'Bucket': bucket, 'Name': photo}})

    textDetections = response['TextDetections']
    print('Detected text\n----------')
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
    return len(textDetections)

def main():
    bucket = 'water-meter-images-test'
    photo = 'watermeter.jpeg'
    text_count = detect_text(photo, bucket)
    print("Text detected: " + str(text_count))

if __name__ == "__main__":
    main()