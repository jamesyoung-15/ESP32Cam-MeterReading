import json
import urllib.parse
import boto3

print('Loading function')

s3 = boto3.client('s3')
rekognition_client = boto3.client('rekognition')


def lambda_handler(event, context):
    print("Received event: " + json.dumps(event, indent=2))

    # Get the object from the event and show its content type
    bucket = event['Records'][0]['s3']['bucket']['name']
    key = urllib.parse.unquote_plus(event['Records'][0]['s3']['object']['key'], encoding='utf-8')
    try:
        # response = s3.get_object(Bucket=bucket, Key=key)
        # print("CONTENT TYPE: " + response['ContentType'])
        # return response['ContentType']
        response = rekognition_client.detect_text(Image={'S3Object': {'Bucket': bucket, 'Name': key}})
        textDetections = response['TextDetections']
        print('Detected text\n----------')
        for text in textDetections:
            print('Detected text:' + text['DetectedText'])
            print('Confidence: ' + "{:.2f}".format(text['Confidence']) + "%")
            print('Id: {}'.format(text['Id']))
            if 'ParentId' in text:
                print('Parent Id: {}'.format(text['ParentId']))
            print('Type:' + text['Type'])
            print()
        return len(textDetections)
    
    except Exception as e:
        print(e)
        print('Error getting object {} from bucket {}. Make sure they exist and your bucket is in the same region as this function.'.format(key, bucket))
        raise e
              
