import cv2
import numpy as np
import pytesseract

# Load the image
img = cv2.imread("awsdigits.jpg")


# Displaying digits and OCR
txt = pytesseract.image_to_string(img, lang='eng',  \
           config='--psm 10 --oem 3 -c tessedit_char_whitelist=0123456789')
print(''.join(t for t in txt if t.isalnum()))
cv2.imshow("res", img)
cv2.waitKey(0)
