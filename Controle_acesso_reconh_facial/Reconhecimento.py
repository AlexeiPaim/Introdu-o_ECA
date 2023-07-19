import cv2
import face_recognition
import os
import serial
import time



# Configuração da porta serial
porta_serial = serial.Serial('COM6', 4800)  # Substitua 'COM6' pela porta serial correta do ESP32 e defina a velocidade de comunicação adequada

# Função para encoding das imagens
def findEncodings(images):
    encodeList = []
    for img in images:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        encode = face_recognition.face_encodings(img)[0]
        encodeList.append(encode)
    return encodeList
''
# Caminho dos arquivos de foto de referência
path = 'Pessoas'
images = []
classNames = []
myList = os.listdir(path)

# Obter o nome dos arquivos
for cl in myList:
    curImg = cv2.imread(f'{path}/{cl}')
    images.append(curImg)
    classNames.append(os.path.splitext(cl)[0])

# Nomes dos rostos
print(classNames)

# Encoding dos rostos
rostosLista = findEncodings(images)
print('Encoding Completo')

cap = cv2.VideoCapture(0)
escala = 0.25
tamanho = 4
font = 0.75

# Loop para usar a webcam
while True:
    success, img = cap.read()
    imgPeq = cv2.resize(img, (0, 0), None, escala, escala)  # Mudar escala de tamanho
    imgPeq = cv2.cvtColor(imgPeq, cv2.COLOR_BGR2RGB)    # Manter escala RGB

    faceCam = face_recognition.face_locations(imgPeq)   # Encontrar face
    encodeCam = face_recognition.face_encodings(imgPeq, faceCam)    # Encoding a face

    for encodeFace, faceLoc in zip(encodeCam, faceCam):
        matches = face_recognition.compare_faces(rostosLista, encodeFace)   # Verificar o rosto com referências
        name = "Desconhecido"  # Nome padrão é "Desconhecido" se não for identificado

        if True in matches:
            matchIndex = matches.index(True) # Posição do True

            if matches[matchIndex]:
                name = classNames[matchIndex].upper()
                
        # Enviar o nome via comunicação serial
        porta_serial.write(name.encode())
        print(name)
        time.sleep(30)
        
        
        y1, x2, y2, x1 = faceLoc
        y1, x2, y2, x1 = y1 * tamanho, x2 * tamanho, y2 * tamanho, x1 * tamanho
        cv2.rectangle(img, (x1, y1), (x2, y2), (0, 0, 255), 2)
        cv2.rectangle(img, (x1, y2-35), (x2, y2), (0, 0, 255), cv2.FILLED)
        cv2.putText(img, name, (x1+6, y2-6), cv2.FONT_HERSHEY_COMPLEX, font, (255, 255, 255), 2)

    cv2.imshow('Camera', img)
    cv2.waitKey(1)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Fechar a porta serial
porta_serial.close()
cv2.destroyAllWindows()
