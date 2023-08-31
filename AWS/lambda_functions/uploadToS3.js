const AWS = require('aws-sdk');
const sharp = require('sharp');

let s3 = new AWS.S3();


exports.handler = (event, context, callback) => {
    let rotateAngle = parseInt(event.rotateDegrees);
    let encodedImage = event.base64Image;
    let Folder = event.S3Folder;
    let Filename = getFormattedTime();
    let x = parseInt(event.cropLeft);
    let y = parseInt(event.cropTop);
    let width = parseInt(event.cropWidth);
    let height = parseInt(event.cropHeight);
    // console.log("Event is ", event);
    // console.log("Copying event body of ", encodedImage);
    

    console.log(`Cropping dimension: x = ${x}, y = ${y}, height = ${height}, width = ${width}`)

    const imageBuffer = Buffer.from(encodedImage, 'base64');
    sharp(imageBuffer)
    .rotate(rotateAngle)
    .extract({ width: width, height: height, left: x, top: y })
    .toBuffer({ resolveWithObject: true })
    .then(({ data, info }) => {
        // let filePath = "images/" + Folder + "/pic_taken/" + Filename + ".jpg";
        let filePath = "images/pic_taken/" + Filename + ".jpg";
        let params = {
            "Body": data,
            "Bucket": "water-meter-images-test",
            "Key": filePath  
        };
        s3.upload(params, function(err, doc){
            if(err) {
                callback(err, null);
            } else {
                let response = {
                    "statusCode": 200,
                    "headers": {
                        "my_header": "my_value"
                    },
                    "body": JSON.stringify(doc),
                    "isBase64Encoded": false
                };
                callback(null, response);
            }
        });
        
    })

    
};


let getFormattedTime = () => {
    // Set timezone specfically to HK, output looks like: 25/08/2023, 11:04:44
    let timeNow = new Date().toLocaleString("en-GB", {timeZone: "Asia/Hong_Kong"});
    // console.log(timeNow);

    // Split the string to date and time like: 25/08/2023 and 11:04:44
    let splitTime = timeNow.split(" ");
    // console.log(splitTime);

    // split the date to month day year
    let regex = splitTime[0].match(/(\d{1,2})\/(\d{1,2})\/(\d{4})/);
    let newDateFormat = regex[1] + regex[2] + regex[3];

    // replace colon in time
    let newTimeFormat = splitTime[1].replace(/:/g, "");
    // console.log(newTimeFormat);

    // combine to form file name, eg. 25082023-110808
    let fileName = newDateFormat + "-" + newTimeFormat;
    // console.log(fileName);
    return fileName;
    
}