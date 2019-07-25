class Vertex:
    def __init__(self, a, b, c):
        self.__x = a
        self.__y = b
        self. __z = c

    def set_x(self, c):
        self.__x = c

    def set_y(self, c):
        self.__y = c

    def set_z(self, c):
        self.__z = c

    def get_x(self):
        return self.__x

    def get_y(self):
        return self.__y

    def get_z(self):
        return self.__z

    def get_coordinates(self):
        return self.__x, self.__y, self.__z

    def __str__(self):
        s = "x: "+str(self.__x)+" y: "+str(self.__y)+" z: "+str(self.__z)
        return s
