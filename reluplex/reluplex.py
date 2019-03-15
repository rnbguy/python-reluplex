import cReluplex as rp


class Reluplex:
    def __init__(self, numVariables):
        self.capsule = rp.construct(numVariables)

    def setName(self, variable, name):
        rp.setName(self.capsule, variable, name)

    def initializeCell(self, row, column, value):
        rp.initializeCell(self.capsule, row, column, value)

    def markBasic(self, variable):
        rp.markBasic(self.capsule, variable)

    def setLowerBound(self, variable, bound):
        rp.setLowerBound(self.capsule, variable, bound)

    def setUpperBound(self, variable, bound):
        rp.setUpperBound(self.capsule, variable, bound)

    def setReluPair(self, backward, forward):
        rp.setReluPair(self.capsule, backward, forward)

    def getAssignment(self, variable):
        return rp.getAssignment(self.capsule, variable)

    def setLogging(self, flag):
        rp.setLogging(self.capsule, flag)

    def setDumpStates(self, flag):
        rp.setDumpStates(self.capsule, flag)

    def solve(self):
        return rp.solve(self.capsule)

    def __delete__(self):
        rp.delete_object(self.capsule)
