#ifndef MY_HTML_H
#define MY_HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/cropperjs/1.6.0/cropper.css" integrity="sha512-hBT8ir1rdidrjzYcDj50eQ5gglkAc1c7tmzpdkiGydrn3Tt3+cGJ+GdsXLYmwETdStY0JZXbv6p4jgGDQU0fMw==" crossorigin="anonymous" referrerpolicy="no-referrer" />
        <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/@picocss/pico@1/css/pico.min.css">
        <style>
            .header {
                text-align:center;
                margin: 0 auto; 
            }
            main{
                text-align: center;
            }
            .button-container{
                margin: 0 auto;
            }
            .button-container a{
                margin-bottom: 10px;
            }
            .crop-container{
                margin: 20px auto;
            }
            canvas{
                max-width: 100%;
                margin-bottom: 30px;
            }
            #photo {
                width: 100%;
            }
            .hidden{
                display: none;
            }
        </style>
    </head>
    <body>
        <main class="container">
            <section class="header">
                <h2>ESP32-CAM</h2>
                <p>Press take photo to get image from ESP32-Cam, then press check photo to display image.</p>
            </section>
            <section class="button-container grid">
                <a href="#" role="button" id="capture-photo">Take Photo</a>
                <a href="#" role="button" onclick="location.reload()">Check Photo</a>
            </section>
            <section class="crop-container grid">
                <div>
                    <img src="saved-photo" id="photo">
                </div>
                <div>
                    <table role="grid">
                        <thead>
                            <tr>
                                <th scope="col">Crop-Left</th>
                                <th scope="col">Crop-Top</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td id="esp32-crop-left"></td>
                                <td id="esp32-crop-top"></td>
                            </tr>
                        </tbody>
                        <thead>
                            <tr>
                                <th scope="col">Crop-Width</th>
                                <th scope="col">Crop-Height</th>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td id="esp32-crop-width"></td>
                                <td id="esp32-crop-height"></td>
                            </tr>
                        </tbody>
                    </table>
                    <label>Rotate <span id="rotate-degrees">0</span>°<br><input type="range" step="1" min="-90" max="90" value="0" id="rotate-image"></label>
                </div>
            </section>
            <section class="button-container grid">
                <a href="#" role="button" id="crop-button">Preview Crop Photo</a>
                <a href="#" role="button" id="send-crop-data">Save Crop Dimensions</a>
                <a href="#" role="button" id="send-aws">Test Send to AWS</a>
            </section>
            <section class="grid">
                <div id="crop-result"></div>
            </section>
            <section class="button-container grid">
                <a class="hidden" href="#" role="button" id="send-aws-interval">Start Interval Sending to AWS</a>
                <a class="hidden" href="#" role="button" id="stop-aws-interval">Stop Interval Sending to AWS</a>
            </section>
        </main>
        <script src="https://code.jquery.com/jquery-3.7.0.min.js" integrity="sha256-2Pmvv0kuTBOenSvLm6bvfBSSHrUJ+3A7x6P5Ebd07/g=" crossorigin="anonymous"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/cropperjs/1.6.0/cropper.js" integrity="sha512-q9N9RDpXWA3HKu2spOTpZ0j64hL0kfwHQiCgGq9FUuVtTOn7K894cu3YQBtA6rz8T7gbAyuBo+sooxR+/sO4Lg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    </body>
    <script>
        window.addEventListener('DOMContentLoaded', function () {
            let image = document.querySelector('#photo');
            let cropPreview = document.getElementById('crop-button');
            let previewResult = document.getElementById('crop-result');
            let sendCropData = document.getElementById('send-crop-data');
            let sendAWSButton = document.getElementById('send-aws');
            let capturePhoto = document.getElementById('capture-photo');
            var croppedImage = image.src;
            let rotateSlider = document.getElementById('rotate-image');
            let startIntervalSend = document.getElementById('send-aws-interval');
            let stopIntervalSend = document.getElementById('stop-aws-interval');
            let cropper = new Cropper(image, {
                zoomable: false,
                rotatable: true,
                // movable: false, 
                ready: function (event) {
                // Zoom the image to its natural size
                cropper.zoomTo(1);
                },
                
                crop: function (event) {
                    let cropData = cropper.getData();
                    // document.querySelector('#cropper-data').textContent = JSON.stringify(cropper.getData());
                    document.querySelector('#esp32-crop-left').textContent = `${Math.round(cropData.x)}`; 
                    document.querySelector('#esp32-crop-top').textContent = `${Math.round(cropData.y)}`;
                    document.querySelector('#esp32-crop-height').textContent =`${Math.round(cropData.height)}`; 
                    document.querySelector('#esp32-crop-width').textContent =`${Math.round(cropData.width)}`;
                },

                zoom: function (event) {
                    // Keep the image in its natural size
                    if (event.detail.oldRatio === 1) {
                        event.preventDefault();
                    }
                },
            });

            cropPreview.onclick = () => {
                previewResult.innerHTML = '';
                let resultTitle = document.createElement("h3");
                resultTitle.textContent = "Crop Result";
                previewResult.appendChild(resultTitle);
                previewResult.appendChild(cropper.getCroppedCanvas());
                startIntervalSend.classList.remove("hidden");
                stopIntervalSend.classList.remove("hidden");
            };

            sendCropData.onclick = () => {
                // convert crop data and store in json
                let cropData = cropper.getData();
                let cropLeft = Math.round(cropData.x);
                let cropTop = Math.round(cropData.y);
                let cropBottom = Math.round(image.naturalHeight - (cropData.y+cropData.height));
                let cropRight = Math.round(image.naturalWidth - (cropData.x+cropData.width));
                let cropHeight = cropData.height;
                let cropWidth = cropData.width;
                let rotateValue = rotateSlider.value;

                let xhr = new XMLHttpRequest();
                xhr.open('GET', `/cropData?cropLeft=${cropLeft}&cropTop=${cropTop}&cropWidth=${cropWidth}&cropHeight=${cropHeight}&rotate=${rotateValue}`, true);
                xhr.send();
                
            }
            sendAWSButton.onclick = () => {
                console.log("Tell ESP32 to send to AWS");
                let xhr = new XMLHttpRequest();
                xhr.open('GET', "/send-aws", true);
                xhr.send();
            }
            capturePhoto.onclick = () => {
                let xhr = new XMLHttpRequest();
                xhr.open('GET', "/capture", true);
                xhr.send();
            }
            rotateSlider.onchange = () => {
                document.getElementById("rotate-degrees").innerHTML = rotateSlider.value;
                console.log("rotate ", rotateSlider.value);
                cropper.rotateTo(rotateSlider.value);
            }
            startIntervalSend.onclick = () => {
                let xhr = new XMLHttpRequest();
                xhr.open('GET', "/startInterval", true);
                xhr.send();
            }
            stopIntervalSend.onclick = () => {
                let xhr = new XMLHttpRequest();
                xhr.open('GET', "/stopInterval", true);
                xhr.send();
            }
        });
    </script>
</html>
)rawliteral";

#endif