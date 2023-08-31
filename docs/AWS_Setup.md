# AWS Setup
Make sure you have an AWS account. If you only have a root account, it is recommended to create an IAM user instead of using root.

## Create AWS IAM User (if only have root)
Skip this if you already have an IAM User (but make sure you have correct permissions to Rekognition, Lambda, API Gateway, S3, DynamoDB) or want to use root.

This is for quick setup, for better setup see [AWS Guide](https://docs.aws.amazon.com/IAM/latest/UserGuide/id_users_create.html).

1. In AWS Management Console, enter `IAM` in the search bar on the top and select `IAM` under Services. 

2. Click on `users` on the left bar, then click `create users`.

3. Enter a username, and select the `Provide user access to the – AWS Management Console` button. For quicker setup, click `I want to create an IAM User`. Below you can configure whether to have custom password and so on. Click next when done.

4. Click `Create Group`, enter a user group name, then for quick setup click `AdministratorAccess`. If you want more control over user permission only give permissions to services used in this project (Rekognition, Lambda, API Gateway, S3, DynamoDB).

5. Click next and create user. Download the credentials.

## Setup Lambda Functions
The project uses two Lambda functions. The first function is triggered when an image received from ESP32-CAM, where it will decode the image, crop the image with Sharp, and then store it in a S3 Bucket. Since this part uses Sharp which isn't included in Lambda, a Lambda layer is needed.

The second function is triggered when the image is stored in the S3 Bucket. Here, the function will call Rekognition to perform text detection, get the meter reading based on that, then store it in a DynamoDB table. Again we will need another layer for Pillow.

### ESP32-CAM-Image-to-S3 Lambda Function
1. To setup, search and go to Lambda service. 

2. Press `create a function` and give it a name. 

3. Select `Node JS 16.x` runtime (as my code isn't up to date newest Node). 

4. Copy and paste the code. The code is [here](../AWS/lambda_functions/uploadToS3.js). 

5. Press `deploy`.

6. We need to add a Lambda layer for Sharp. Go to [this repo](https://github.com/Umkus/lambda-layer-sharp/releases) and download the latest `sharp-layer.zip`.

7. Go back to AWS Lambda. On the left bar, press `layers` under `Additional Resources`. 

7. Press `Create Layer`, give it a name, press `upload` and upload the `sharp-layer.zip`. Then press `create`.

8. Go to `Functions` in left bar of Lambda and press into the function you created earlier. Scroll down till you see layers, press `Add a layer`, press `custom layers`, then select the Sharp layer and add it.

### S3-Trigger-Rekognition Lambda Function
1. To setup, search and go to Lambda service. 

2. Press `create a function` and give it a name. 

3. Select `Python 3.8` runtime (Pillow layer used only supports 3.8).

4. 

## API Gateway


## DynamoDB

## 