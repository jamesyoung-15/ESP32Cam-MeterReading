import easyocr

reader = easyocr.Reader(['en'])

result = reader.readtext('test2.png', allowlist ='0123456789')

print(result)