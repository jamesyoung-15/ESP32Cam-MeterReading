var AWS = require('aws-sdk');

const bucket = 'water-meter-images-test' // the bucketname without s3://
const photo  = 'watermeter.jpeg' // the name of file

const config = new AWS.Config({
  accessKeyId: process.env.AWS_ACCESS_KEY_ID,
  secretAccessKey: process.env.AWS_SECRET_ACCESS_KEY,
}) 
AWS.config.update({region:'us-east-1'});
const client = new AWS.Rekognition();
const params = {
  Filters: { 
    RegionsOfInterest: [ 
        { 
          BoundingBox: { 
              Height: 0.055555615574121475,
              Left: 0.4526198208332062,
              Top: 0.795634925365448,
              Width: 0.05462656542658806
          },
          Polygon: [ 
              { 
                X: 0.32,
                Y: 0.6
              }
          ]
        }
    ],
    },
  Image: {
    S3Object: {
      Bucket: bucket,
      Name: photo
    },
  }
}
client.detectText(params, function(err, response) {
  if (err) {
    console.log(err, err.stack); // handle error if an error occurred
  } else {
    console.log(`Detected Text for: ${photo}`)
    console.log(response)
    response.TextDetections.forEach(label => {
      console.log(`Detected Text: ${label.DetectedText}`),
      console.log(`Type: ${label.Type}`),
      console.log(`ID: ${label.Id}`),
      console.log(`Parent ID: ${label.ParentId}`),
      console.log(`Confidence: ${label.Confidence}`),
      console.log(`Polygon: `)
      console.log(label.Geometry.Polygon)
      console.log(`BoundingBox: `)
      console.log(label.Geometry.BoundingBox)
    } 
    )
  } 
});