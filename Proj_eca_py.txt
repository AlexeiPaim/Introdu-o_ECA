import cv2
import mediapipe as mp
import serial

video = cv2.VideoCapture(0)

hands = mp.solutions.hands
Hands = hands.Hands(max_num_hands=1)
mpDraw = mp.solutions.drawing_utils

# Inicializar a comunicação serial com o Arduino
arduino = serial.Serial('COM3', 9600)  # Substitua 'COM3' pela porta serial correta

while True:
    success, img = video.read()
    
    if not success:
        print("Não foi possível ler o frame da câmera. Verifique se a câmera está conectada corretamente.")
        break
    
    frameRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = Hands.process(frameRGB)
    handPoints = results.multi_hand_landmarks

    h, w, _ = img.shape
    pontos = []
    if handPoints:
        for points in handPoints:
            mpDraw.draw_landmarks(img, points, hands.HAND_CONNECTIONS)
            # Podemos enumerar esses pontos da seguinte forma
            for id, cord in enumerate(points.landmark):
                cx, cy = int(cord.x * w), int(cord.y * h)
                cv2.putText(img, str(id), (cx, cy + 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 0), 2)
                pontos.append((cx, cy))

            dedos = [8, 12, 16, 20]
            contador = 0
            if pontos:
                if pontos[4][0] < pontos[3][0]:
                    contador += 1
                for x in dedos:
                    if pontos[x][1] < pontos[x - 2][1]:
                        contador += 1

            cv2.rectangle(img, (80, 10), (200, 110), (0, 0, 255), -1)
            cv2.putText(img, str(contador), (100, 100), cv2.FONT_HERSHEY_SIMPLEX, 4, (0, 255, 0), 5)

            # Enviar o valor do contador para o Arduino
            arduino.write((str(contador) + '\n').encode())

    cv2.imshow('Imagem', img)
    if cv2.waitKey(1) == ord('q'):
        break

video.release()
cv2.destroyAllWindows()
