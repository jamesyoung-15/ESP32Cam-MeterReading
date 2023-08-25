import easyocr

reader = easyocr.Reader(['en'])

result = reader.readtext('digits.png', allowlist ='0123456789')

print(result)