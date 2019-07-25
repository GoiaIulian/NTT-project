import pygame
from pygame.locals import *
from Cube import *
from OpenGL.GLU import *
from pyquaternion import Quaternion
import serial


def draw_text(position, text_string):
    font = pygame.font.Font(None, 14)
    text_surface = font.render(text_string, True, (255, 255, 255, 255), (0, 0, 0, 255))
    text_data = pygame.image.tostring(text_surface, "RGBA", True)
    glRasterPos3d(*position)
    glDrawPixels(text_surface.get_width(), text_surface.get_height(), GL_RGBA, GL_UNSIGNED_BYTE, text_data)


def main():
    pygame.init()
    display = (800, 600)

    c = Cube()
    pygame.display.set_mode(display, DOUBLEBUF|OPENGL)

    gluPerspective(45, (display[0]/display[1]), 0.1, 50.0)
    glTranslatef(0.0, 0.0, -5)
    ser = serial.Serial(
        port='COM6',
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0xFFFF)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        line = ser.readline()
        line = line.rstrip(b'\r\n')
        elements = line.decode("utf-8").split(';')
        s = ""
        if 3 < elements.__len__() < 5:
            q = Quaternion(elements[0], elements[1], elements[2], elements[3])
            glMultMatrixf(q.transformation_matrix)
            s = elements[0] + " " + elements[1] + " " + elements[2] + " " + elements[3]
        # print(elements)
        print(s)

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        c.draw_cube()
        # draw_text((10, 10, 10), s)
        pygame.display.flip()
        pygame.time.wait(10)


main()
