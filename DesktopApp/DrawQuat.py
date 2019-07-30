import pygame
import math
from OpenGL.GL import *
from OpenGL.GLU import *
from pygame.locals import *
import serial


# quaternions = (
#     [0.01, 0.00, 1.00, 0.00],
#     [0.02, 0.00, 1.00, 0.00],
#     [0.03, 0.00, 1.00, 0.00],
#     [0.04, 0.00, 1.00, 0.00],
#     [0.05, 0.00, 1.00, 0.00],
#     [0.06, 0.00, 1.00, 0.00],
#     [0.07, 0.00, 1.00, 0.00],
#     [0.08, 0.00, 1.00, 0.00],
#     [0.09, 0.00, 1.00, 0.00],
#     [0.01, 1.00, 0.00, 0.00],
#     [0.02, 1.00, 0.00, 0.00],
#     [0.03, 1.00, 0.00, 0.00],
#     [0.04, 1.00, 0.00, 0.00],
#     [0.05, 1.00, 0.00, 0.00],
#     [0.06, 1.00, 0.00, 0.00],
#     [0.07, 1.00, 0.00, 0.00],
#     [0.08, 1.00, 0.00, 0.00],
#     [0.09, 1.00, 0.00, 0.00]
# )


def main():
    video_flags = OPENGL | DOUBLEBUF
    pygame.init()
    width = 640
    height = 480
    i = 0

    pygame.display.set_mode((width, height), video_flags)
    pygame.display.set_caption("Sensor Orientation")

    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45, 1.0 * width / height, 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    glShadeModel(GL_SMOOTH)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClearDepth(1.0)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

    ser = serial.Serial(
        port='COM6',
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0xFFFF)

    while 1:
        event = pygame.event.poll()
        if event.type == QUIT:
            # ser.close()
            pygame.quit()
            quit()
        if i > 14:
            i = 0

        line = ser.readline()
        line = line.rstrip(b'\r\n')
        elements = line.decode("utf-8").split(';')
        if elements.__len__() == 5:
            draw(float(elements[0]), float(elements[1]), float(elements[2]), float(elements[3]), float(elements[4]))

        pygame.display.flip()


def draw(w, nx, ny, nz, t):
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    glTranslatef(0, 0.0, -7.0)

    draw_text((-2.6, -1.6, 2), "Temperature: %.2f °C" % t, 16)
    draw_text((-2.6, -1.8, 2), "w: %f, i: %f, j: %f, k: %f" % (w, nx, ny, nz), 16)
    glRotatef(2 * math.acos(w) * 180.00 / math.pi, -1 * nx, nz, ny)

    glBegin(GL_QUADS)
    glColor3f(0.5, 0.5, 0.5)
    glVertex3f(1.0, 0.1, -1.0)
    glVertex3f(-1.0, 0.1, -1.0)
    glVertex3f(-1.0, 0.1, 1.0)
    glVertex3f(1.0, 0.1, 1.0)

    glColor3f(0.5, 0.5, 0.5)
    glVertex3f(1.0, -0.1, 1.0)
    glVertex3f(-1.0, -0.1, 1.0)
    glVertex3f(-1.0, -0.1, -1.0)
    glVertex3f(1.0, -0.1, -1.0)

    glColor3f(1.0, 1.0, 1.0)
    glVertex3f(1.0, 0.1, 1.0)
    glVertex3f(-1.0, 0.1, 1.0)
    glVertex3f(-1.0, -0.1, 1.0)
    glVertex3f(1.0, -0.1, 1.0)

    glColor3f(1.0, 1.0, 1.0)
    glVertex3f(1.0, -0.1, -1.0)
    glVertex3f(-1.0, -0.1, -1.0)
    glVertex3f(-1.0, 0.1, -1.0)
    glVertex3f(1.0, 0.1, -1.0)

    glColor3f(0.5, 1.0, 1.0)
    glVertex3f(-1.0, 0.1, 1.0)
    glVertex3f(-1.0, 0.1, -1.0)
    glVertex3f(-1.0, -0.1, -1.0)
    glVertex3f(-1.0, -0.1, 1.0)

    glColor3f(0.5, 1.0, 1.0)
    glVertex3f(1.0, 0.1, -1.0)
    glVertex3f(1.0, 0.1, 1.0)
    glVertex3f(1.0, -0.1, 1.0)
    glVertex3f(1.0, -0.1, -1.0)
    glEnd()


def draw_text(position, text_string, size):
    font = pygame.font.SysFont("Courier", size, True)
    text_surface = font.render(text_string, True, (255, 255, 255, 255), (0, 0, 0, 255))
    text_data = pygame.image.tostring(text_surface, "RGBA", True)
    glRasterPos3d(*position)
    glDrawPixels(text_surface.get_width(), text_surface.get_height(), GL_RGBA, GL_UNSIGNED_BYTE, text_data)


if __name__ == '__main__':
    main()
