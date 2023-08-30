import easyocr

reader = easyocr.Reader(['en'])

result = reader.readtext('awsdigits.jpg', allowlist ='0123456789')

print(result)