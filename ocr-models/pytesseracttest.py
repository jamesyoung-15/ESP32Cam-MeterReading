import cv2
import numpy as np
import pytesseract

# Load the image
img = cv2.imread("digits.png")

# Color-segmentation to get binary mask
lwr = np.array([43, 0, 71])
upr = np.array([103, 255, 130])
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
msk = cv2.inRange(hsv, lwr, upr)
cv2.imwrite("msk.png", msk)

# Extract digits
krn = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 3))
dlt = cv2.dilate(msk, krn, iterations=5)
res = 255 - cv2.bitwise_and(dlt, msk)
cv2.imwrite("res.png", res)

# Displaying digits and OCR
txt = pytesseract.image_to_string(res, config="--psm 6 digits")
print(''.join(t for t in txt if t.isalnum()))
cv2.imshow("res", res)
cv2.waitKey(0)
