input_file = 'vcg_signal2.txt'

with open(input_file, 'r') as f:
    lines = f.readlines()

# Formate o conteúdo do arquivo em uma única string
formatted_content = ""
for line in lines:
    formatt = '"'+line+'",'
    formatt = formatt.replace("\t"," ")
    formatt = formatt.replace("\n","")
    formatt += "\n"
    formatted_content = formatted_content+ formatt
print(formatted_content)

arduino_code = f'const String fileData = "{formatted_content}";\n'

# Salve o código em um arquivo .ino
with open("vcg2_formated.txt", 'w') as f:
    f.write(arduino_code)