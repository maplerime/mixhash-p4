# Level 2 module base
from RegisterControl import regmap
import sys
import os
from credo import *
from importlib import import_module
# Level function objects
class L2Func(object):
    pass

# Inject function into RegHive. The type of lane_obj should be 
# basically compatible with RegHive.
# 
def inject_func(hive_obj, func):
    if hive_obj.__class__ != FuncHive:
        FuncHive.transform(hive_obj)
    hive_obj.inject(func)

def inject_hive(hive):
    report = permMode.report()
    name = hive._gname
    for _, value in report.items():
        for key1, value1 in value.items():
            if (key1 == name) or (key1.lower() == name):
                for func in value1:
                    inject_custom(hive, func[0], func[1])

def inject_custom(hive_obj, name, func):
    if hive_obj.__class__ != FuncHive:
        FuncHive.transform(hive_obj)
    hive_obj.inject_custom(name, func)

def reload_func(func):
    filePath = func.funcFile
    files = os.path.split(filePath)
    if files[0] not in sys.path:
        sys.path.append(files[0])
    moduleImport = __import__(files[1][:-3])
    # exec("import %s as moduleImport"%files[1][:-3])
    reload(moduleImport)
    # exec ("reload (%s)" % files[1][:-3])
    funcNew = getattr(moduleImport, func.funcName)
    inject_func(func.hiveObj, funcNew)

class FuncHive(regmap.RegHive):
    @staticmethod
    def transform(self):
        object.__setattr__(self, "_funcs", dict())
        object.__setattr__(self, "__class__", FuncHive)
        for l in self._lanes:
            FuncLane.transform(l)

    def __getattr__(self, attr):
        if attr in self._funcs:
            f = self._funcs[attr]
            def func_hive_call(lane_no, *args, **kwargs):
                if isinstance(lane_no, list):
                    return f(lane_no, *args, **kwargs)
                return f(self._lanes[lane_no], *args, **kwargs)
            func_hive_call.__doc__=f.__doc__
            func_hive_call.funcFile = f.__code__.co_filename
            func_hive_call.funcName = f.__name__
            func_hive_call.hiveObj = self
            return func_hive_call
        else:
            return super(FuncHive, self).__getattr__(attr)
    
    def inject(self, func):
        name = func.__name__
        self._funcs[name] = func

    def inject_custom(self, name, func):
        self._funcs[name] = func

class FuncLane(regmap.RegLane):
    @staticmethod
    def transform(self):
        object.__setattr__(self, "__class__", FuncLane)

    def __getattr__(self, attr):
        if attr in self._hive._funcs:
            f = self._hive._funcs[attr]
            def func_lane_call(*args, **kwargs):
                return f(self, *args, **kwargs)
            return func_lane_call
        else:
            return super(FuncLane, self).__getattr__(attr)

def reload_file(chip, filename):
    if os.path.isfile(filename):
        reinjectList = []
        global selectorList
        importFilename = filename.replace('..', '')
        importFilename = importFilename.replace('/', '.')
        if importFilename[0] == '.':
            importFilename = importFilename[1:]
        importFilename = importFilename[:-3]
        basename, _ = os.path.splitext(os.path.basename(filename))
        tempDecorator = permMode(None)
        report = tempDecorator.report()
        removeDict = {}
        for key, value in report[basename].items():
            info1 = (basename, key)
            for info2 in value:
                removeDict.setdefault(key, []).append(info2[0])
                info = info1 + info2
                tempDecorator.removeEntry(info)
        for key, value in chip.__dict__.items():
            if key in removeDict.keys():
                for i in removeDict[key]:
                    del value._funcs[i]   
        focusModule = import_module(importFilename)
        reload(focusModule)
        report = permMode.report()[basename]
        for key, value in chip.__dict__.items():
            if key in report.keys():
                for info in report[key]:
                    inject_custom(value, info[0], info[1])
        namespace = regFuncInit.getNamespace()
        for i in selectorList:
            del namespace[i]
        generateSelectorFunctions(chip)
    else:
        raise Exception('{} is not found'.format(filename))

def reloadAll(chip, g, basename):
    from IPython.lib import deepreload
    tempDecorator = permMode(None)
    report = tempDecorator.report(1)
    removeDict = {}
    for hive in report.keys():
        for info in report[hive]:
            removeDict.setdefault(hive, []).append(info[1])
            tempDecorator.removeEntry((info[0],) + (hive,) + info[1:])

    for key, value in chip.__dict__.items():
        if key in removeDict.keys():
            for i in removeDict[key]:
                try:
                    del value._funcs[i]
                except:
                    pass
    deepreload.reload(__import__(basename), exclude = ('numpy', 'os.path', basename))
    global selectorList
    report = permMode.report(1)
    for key in report.keys():
        key_name = None
        if key.upper() in dir(chip):
            key_name = key.upper()
        else:
            if key.lower() in dir(chip):
                key_name = key.lower()
        if key_name is not None:
            inject_hive(getattr(chip, key_name))
        else:
            print '{} was not reinjected!'.format(key)
    generateMultiLaneFunctions(chip)
    for i in selectorList:
        try:
            del g[i]
        except:
            pass
    regFuncInit.addNamespace(g)
    regFuncInit.addHiveList(g['hiveList'])
    regFuncInit.addInputList(g['inputList'])
    regFuncInit.addModeList(g['modeList'])
    regFuncInit.addNameList(g['nameList'])
    regFuncInit.addgDebugTuning(g['gDebugTuning'])
    generateSelectorFunctions(chip)

def newMultiLaneFunction(hive, core):
    def functionTemplate(index, *args, **kwargs):
        if isinstance(index, list):
            returnlist = []
            for i in index:
                returnlist.append(core(hive[i], *args, **kwargs))
            return returnlist
        else:
            return core(index, *args, **kwargs)
    return functionTemplate

def generateMultiLaneFunctions(chip):
    coreDictionary = permMode.report(5)
    if coreDictionary is not None:
        for info in coreDictionary:
            key_name = None
            key = info[1]
            if key.upper() in dir(chip):
                key_name = key.upper()
            else:
                if key.lower() in dir(chip):
                    key_name = key.lower()
            if key_name is not None:
                hiveobj = getattr(chip, key_name)
                inject_custom(hiveobj, info[2], 
                              newMultiLaneFunction(hiveobj, info[3]))
            else:
                print '{} selector function failed to inject!'.format(info[2])
            

def newSelectorFunction(chip, corename):
    def functionTemplate(index, *args, **kwargs):
        modeList = regFuncInit.getModeList()
        if isinstance(index, int):
            index = [index]
        if isinstance(index, list):
            returnlist = []
            for i in index:
                core = getattr(getattr(chip, modeList[i].upper()), corename)
                returnlist.append(core(i, *args, **kwargs))
            return returnlist
    return functionTemplate

def generateSelectorFunctions(chip):
    coreDictionary = permMode.report(1)
    if coreDictionary == {}:
        return
    hiveList = regFuncInit.getHiveList()
    global selectorList
    selectorList = []
    nameArray = [[]] * len(hiveList)
    for i in range(len(hiveList)):
        infoList = coreDictionary[hiveList[i]]
        nameArray[i] = (len(infoList), map((lambda info: info[1]), infoList))
    minArray = nameArray.pop(nameArray.index(min(nameArray, key=(lambda x: x[0]))))
    for func in minArray[1]:
        boolList = [False] * len(nameArray)
        for i in range(len(nameArray)):
            boolList[i] = func in nameArray[i][1]
        if reduce((lambda x, y: x and y), boolList):
            namespace = regFuncInit.getNamespace()
            namespace[func] = newSelectorFunction(chip, func)
            selectorList.append(func)

selectorList = []

class regFuncInit(object):
    hiveList = None
    inputList = None
    modeList = None
    nameList = None
    namespace = None
    gDebugTuning = None
    gDevice = None
    gEncodingMode = None

    @staticmethod
    def addHiveList(hiveList):
        regFuncInit.hiveList = hiveList
    @staticmethod
    def getHiveList():
        return regFuncInit.hiveList

    @staticmethod
    def addInputList(inputList):
        regFuncInit.inputList = inputList
    @staticmethod
    def getInputList():
        return regFuncInit.inputList

    @staticmethod
    def addModeList(modeList):
        regFuncInit.modeList = modeList
    @staticmethod
    def getModeList():
        return regFuncInit.modeList
    @staticmethod
    def updateModeList():
        regFuncInit.namespace['modeList'] = regFuncInit.modeList

    @staticmethod
    def addNameList(nameList):
        regFuncInit.nameList = nameList
    @staticmethod
    def getNameList():
        return regFuncInit.nameList

    @staticmethod
    def addNamespace(namespace):
        regFuncInit.namespace = namespace
    @staticmethod
    def getNamespace():
        return regFuncInit.namespace

    @staticmethod
    def addgDebugTuning(gDebugTuning):
        regFuncInit.gDebugTuning = gDebugTuning
    @staticmethod
    def getgDebugTuning():
        #return regFuncInit.gDebugTuning[0]
	return regFuncInit.gDebugTuning

    #@staticmethod
    #def addgDevice(gDevice):
    #    regFuncInit.gDevice = gDevice
    #@staticmethod
    #def getgDevice():
    #    return regFuncInit.gDevice[0]

    #@staticmethod
    #def addgEncodingMode(gEncodingMode):
    #    regFuncInit.gEncodingMode = gEncodingMode
    #@staticmethod
    #def getgEncodingMode():
    #    return regFuncInit.gEncodingMode
    #@staticmethod
    #def updategEncodingMode():
    #    for i in range(len(regFuncInit.modeList)):
    #        pass
    #        if regFuncInit.modeList[i] == 'off':
    #        elif regFuncInit.modeList[i] == 'pam4':
    #        else:
    #    regFuncInit.namespace['modeList'] = regFuncInit.modeList

    #    if rreg([0x0ff,[12]],ln)==0 or rreg([0x1ff,[12]],ln)==0: # Lane's bandgap is OFF
    #        #print ("\n Device %d lane %2d is OFF"%(gDevice,ln)),
    #        data_rate= 1.0
    #        gEncodingMode[gDevice][ln] = ['off',data_rate]
    #        lane_mode_list[ln] = 'off'

    #    elif rreg([0xb0,[1]],ln) == 0 and rreg([0x41,[15]],ln) == 1:
    #        #print ("\n Device %d lane %2d is PAM4"%(gDevice,ln)),
    #        data_rate= get_lane_pll(ln)[ln][0][0]
    #        gEncodingMode[gDevice][ln] = ['pam4',data_rate]
    #        lane_mode_list[ln] = 'pam4'
    #    else:
    #        #print ("\n Device %d lane %2d is NRZ"%(gDevice,ln)),
    #        data_rate= get_lane_pll(ln)[ln][0][0]
    #        gEncodingMode[gDevice][ln] = ['nrz',data_rate]
    #        lane_mode_list[ln] = 'nrz'
            
    #return gEncodingMode
