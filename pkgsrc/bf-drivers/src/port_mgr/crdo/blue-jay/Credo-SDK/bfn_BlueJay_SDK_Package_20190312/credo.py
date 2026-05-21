import os

class permMode(object):
    classification = set()
    filename = ''
    hivename = ''

    def __init__(self, access, funcName = None, multiLane = False):
        if access is not None:
            self.access = access
            self.multiLane = multiLane
            if isinstance(funcName, bool):
                raise Exception('Custom Function Name can not be Boolean!\nSet MultiLane?')
            if funcName is not None:
                self.multiLane = True
            self.funcName = funcName
    
    def __call__(self, function):
        def new_func(func, val):
            def _new_func(*args, **kwargs):
                garbage = val
                return func(*args, **kwargs)
            return _new_func
        def badFunction(lane_obj):
            print ('Access Denied')
        def accessCheck(*args, **kwargs):
            return function(*args, **kwargs)
        if self.funcName is None:
            self.funcName = function.__name__
        if self.access == 'open':
            f = accessCheck
        if self.access == 'closed' or self.access == 'private':
            f =  badFunction
        if isinstance(self.hivename, list):
            i = 0
            for hive in self.hivename:
                self.classification.add((self.filename, hive, 
                                         self.funcName, new_func(f, i), 
                                         self.access, self.multiLane))
                i += 1
        else:
            self.classification.add((self.filename, self.hivename, 
                                     self.funcName, f, 
                                     self.access, self.multiLane))
        return accessCheck

    @staticmethod
    def report(reportType = 0):
        def organize(entryList, index):
            def insert(entry):
                entry = list(entry)
                key = entry.pop(index)
                newReport.setdefault(key, []).append(tuple(entry))
            entryList = list(entryList)
            newReport = {}
            map((lambda entry: insert(entry)), entryList)
            return newReport
        if reportType == 0:
            newReport = organize(permMode.classification, reportType)
            for key, value in newReport.items():
                newReport[key] = organize(value, 0)
            return newReport
        if reportType == 1:
            return organize(permMode.classification, reportType) 
        if reportType == 5:
            if True in organize(permMode.classification, reportType).keys():
                return organize(permMode.classification, reportType)[True]
            else:
                return None
        return permMode.classification

    @staticmethod
    def setFilename(file):
        currentFilename = os.path.basename(os.path.realpath(file))
        name, _ = os.path.splitext(currentFilename)
        permMode.filename = name

    @staticmethod
    def setHivename(name):
        permMode.hivename = name

    def removeEntry(self, entry):
        self.classification.remove(entry)

class public(permMode):
    def __init__(self, funcName = None, multiLane = False):
        permMode.__init__(self, 'open', funcName, multiLane)

class closed(permMode):
    def __init__(self, funcName = None, multiLane = False):
        permMode.__init__(self, 'closed', funcName, multiLane)

class private(permMode):
    def __init__(self, funcName = None, multiLane = False):
        permMode.__init__(self, 'private', funcName, multiLane)
