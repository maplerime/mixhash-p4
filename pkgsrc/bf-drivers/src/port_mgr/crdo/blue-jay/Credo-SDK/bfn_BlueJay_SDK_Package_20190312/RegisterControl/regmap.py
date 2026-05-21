import xlrd
import re
import string
import json
import os
import csv

# try:
#     from regs import *
# except Exception:
#     pass

version_sheet_name = "Version"
index_sheet_name = "Index"
#index_sheet_name = "INDEX_gp4_MCU"
index_first_field = "Sheet"
sheet_first_field = "Register"
re_RESERVED = re.compile(r"^RESERVED")


re_addr = re.compile(r'^([0-9a-fA-F]{4})\.(\d+)(?::(\d+))?$')
re_regname = re.compile(r'^(\w+)(?:\[(\d+)(?::(\d+))?\])?$')
re_owen = re.compile(r'OWEN_')
tr_sheet_name = string.maketrans("+/-* ", "_____")
re_number = re.compile(r'(\d+)')
re_def_value = re.compile(r"^(\d+)'(h|b|d)([0-9a-fA-F]+)$")
re_sign_1 = re.compile(r'format\s*S\d+\.\d+',flags=re.I)
re_sign_2 = re.compile(r'S\d+\.\d+\s*format',flags=re.I)
re_sign = re.compile(r'^S\d+\.\d+',flags=re.I)

__all__ = ["load_regmap", "inject_reg", "convert_script", "saveAllRegsToFile", "saveUsedRegsToFile",
           "getProjectName", "load_Regs", "addRegisterToText", "addRegistersToTextByFile", "saveToCSV"]

def sheet_transform(sheet_name):
    return sheet_name.translate(tr_sheet_name)


class RegBase(object):
    # def __init__(self, name, RO=False):
    #     self.name = name
    #     self.RO = True
    #     if RO == "RW" or RO == "R/W" or RO == "SC":
    #         self.RO = False

    def __init__(self, name, RO=False):
        self.name = name
        self.RO = RO

    def read(self, lane, rdfunc):
        raise NotImplementedError

    def write(self, value, lane, rdfunc, wrfunc):
        raise NotImplementedError

class Reg(RegBase):
    """Regular register. Support read and write function."""
    def __init__(self, name, nob, reg_lsb, addr, lsb, rw, def_value, bases, sign=False):
        if rw == "RW" or rw == "R/W" or rw == "SC":
            RO = False
        else:
            RO = True
        # super(Reg, self).__init__(name, rw=="R")
        super(Reg, self).__init__(name, RO)
        self.width = [nob]
        self.reg_lsb = [reg_lsb]
        self.addr = [addr]
        self.lsb = [lsb]
        self.def_value = [def_value]
        self.parts = 1
        self.bases = bases
        self.sign = sign
        self.bUsed = False

    def convertToDict(self, bSaveUsed):
        if self.bUsed  is False and bSaveUsed is True:
            return  None
        dict = {}
        dict.update(self.__dict__)
        return dict

    def info(self):
        dictInfo = {}
        dictInfo['addr'] = self.addr
        dictInfo['lsb'] = self.lsb
        dictInfo['width'] = self.width
        dictInfo['defaultValue'] = self.def_value
        dictInfo['readOnly'] = self.RO
        self.bUsed = True
        return dictInfo

    def add_to_hive(self, hive):
        if self.name in hive.keys():
            # multi-part register
            old = hive[self.name]
            old.width += self.width
            old.reg_lsb += self.reg_lsb
            old.addr += self.addr
            old.lsb += self.lsb
            old.def_value += self.def_value
            old.parts += 1
            if self.sign != old.sign:
                print "Warning:the sign of register:%s conflict!"%self.name
                old.sign = False
            return old, len(old.width)-1
        else:
            hive[self.name] = self
            return self, 0

    # def read(self, lane, rdfunc):
    #     base = self.bases[lane]
    #     value = 0L
    #     for i in range(self.parts):
    #         mask = (1<<self.width[i]) - 1
    #         v = rdfunc(self.addr[i]+base)
    #         v = (v>>self.lsb[i]) & mask
    #         value |= v<<self.reg_lsb[i]
    #     return value
    def convertToSign(self, value, width):
        result = value
        mask = (1<< (width - 1))
        if (value & mask) != 0:
            result = 0-((mask << 1) - value)
        return result

    def read(self, lane, rdfunc):
        self.bUsed = True
        base = self.bases[lane]
        value = 0L
        width = 0
        for i in range(self.parts):
            mask = (1<<self.width[i]) - 1
            v = rdfunc(self.addr[i]+base)
            v = (v>>self.lsb[i]) & mask
            value |= v<<self.reg_lsb[i]
            width += self.width[i]
        if self.sign is True:
            value = self.convertToSign(value, width)
        return value

    def getRange(self):
        w = 0
        for i in range(self.parts):
            w+=self.width[i]
        min = 0
        max = 0
        if self.sign is True:
            min = 0 - (1 << (w-1))
            max = (1 << (w-1)) -1
        else:
            min = 0
            max = (1 << w) - 1

        return min, max

    def write(self, value, lane, rdfunc, wrfunc):
        self.bUsed = True
        min, max = self.getRange()
        if value > max or value < min:
            print "Warning:the write value is out of range(%d, %d)"%(min, max)
        if self.RO:
            print "Warning: Trying to modify read-only register %s" % self.name
        base = self.bases[lane]
        for i in range(self.parts):
            mask = (1<<self.width[i]) - 1
            v = ((value>>self.reg_lsb[i]) & mask) << self.lsb[i]
            addr = self.addr[i]+base
            if self.width[i] != 16:
                # R-M-W
                old = rdfunc(addr)
                old &= ~(mask << self.lsb[i])
                wrfunc(addr, old | v)
            else:
                wrfunc(addr, v)

    def __str__(self):
        s = ""
        for i in range(self.parts):
            w = self.width[i]
            if w == 1:
                h = "0x%04x.%d" % (self.addr[i], self.lsb[i])
                c = "%s[%d]" % (self.name, self.reg_lsb[i])
            else:
                h = "0x%04x.%d:%d" % (self.addr[i], self.lsb[i]+w-1, self.lsb[i])
                c = "%s[%d:%d]" % (self.name, self.reg_lsb[i]+w-1, self.reg_lsb[i])

            s += "%-12s = %s\n" % (h, c)
        if len(self.bases)==1:
            s += "\nBase=0x%04x\n" % self.bases[0]
        else:
            s += "\nBase=[%s]\n" % ", ".join(["0x%04x" % x for x in self.bases])
        return s


class OverwriteReg(RegBase):
    def __init__(self, name, owen, ow):
        super(OverwriteReg, self).__init__(name)
        self.owen = owen
        self.ow = ow

    def convertToDict(self, bSaveUsed):
        return self.owen.convertToDict(bSaveUsed)

    def read(self, lane, rdfunc):
        if self.owen.read(lane, rdfunc) == 0:
            return None
        return self.ow.read(lane, rdfunc)

    def write(self, value, lane, rdfunc, wrfunc):
        if value is None:
            self.owen.write(0, lane, rdfunc, wrfunc)
        else:
            self.ow.write(value, lane, rdfunc, wrfunc)
            self.owen.write(1, lane, rdfunc, wrfunc)

class DupReg(RegBase):
    def __init__(self, name, dup, dup_list):
        super(DupReg, self).__init__(name)
        self.dup_list = dup_list
        self.msg = "Ambiguity in reference of register %s. Use one of a longer name:\n\t" % name
        self.msg += "\n\t".join(dup_list)

    def convertToDict(self, bSaveUsed):
        return None

    def read(self, lane, rdfunc):
        #raise AttributeError("Ambiguity in reference of reigster %s. Use a longer name: %s"
                #% self.name)
        raise AttributeError(self.msg)

    def write(self, value, lane, rdfunc, wrfunc):
        raise AttributeError("Ambiguity in reference of register %s"
                % self.name)

class ArrayReg(object):
    def __init__(self, name, regs):
        self.name = name
        self.regs = regs

class ArrayRegProxy(object):
    def __init__(self, arrayreg, lane, rdfunc, wrfunc):
        self.arrayreg = arrayreg
        self.lane = lane
        self.rdfunc = rdfunc
        self.wrfunc = wrfunc
        self.bUsed = False

    def setUsed(self):
        if self.bUsed is False:
            for key in self.arrayreg.regs:
                self.arrayreg.regs[key].bUsed = True
            self.bUsed = True

    def info(self, index):
        self.setUsed()
        return self.arrayreg.regs[index].info()

    def __getitem__(self, index):
        if index not in self.arrayreg.regs:
            raise IndexError
        self.setUsed()
        return self.arrayreg.regs[index].read(self.lane, self.rdfunc)

    def __setitem__(self, index, value):
        if index not in self.arrayreg.regs:
            raise IndexError
        self.setUsed()
        return self.arrayreg.regs[index].write(value, self.lane, self.rdfunc, self.wrfunc)

    def __iter__(self):
        self.setUsed()
        return self.arrayreg.regs.__iter__()

class HiveDescriptor(object):
    def __init__(self):
        self.sheets = []
        self.dup = dict()
        self.dup_list = dict()
        self.lanes = None
        self.sheet_index = dict()
        self.base = None

    def add_sheet(self, sheet, lanes, g):
        if self.lanes is not None:
            #print lanes
            #print self.lanes
            
            if self.lanes != lanes:
                raise LookupError("Inconsistent number of lanes in group %s, sheet %s"
                        % (g, sheet.sheet_name))
        else:
            self.lanes = lanes
        self.sheet_index[sheet.sheet_name] = len(self.sheets)
        self.sheets.append(sheet)

    def check_dup(self):
        allkeys = set()
        dupkeys = set()
        for s in self.sheets:
            sheet_keys = set(s.regs.keys())
            d = allkeys & sheet_keys
            dupkeys |= d
            allkeys |= sheet_keys
        if not dupkeys: return
        # Process each dupkeys
        for d in dupkeys:
            snames = []
            reg_new_names = []
            for s in self.sheets:
                if d in s.regs:
                    snames.append(s.sheet_name)
                    reg_new_names.append(sheet_transform(s.sheet_name)+"__"+d)
                    #s.dup.add(d)
            self.dup[d] = snames
            self.dup_list[d] = reg_new_names
            print "Warning: Duplicate register name %s in sheet %s" % (d, ", ".join(snames))


class SheetDescriptor(object):
    def __init__(self, regs, group_regs, sheet_name, base):
        self.regs = regs
        self.group_regs = group_regs
        self.sheet_name = sheet_name
        self.base = base
        self.dup = set()

class RevMap(object):
    def __init__(self):
        self.addr_space = dict()

    def add(self, reg, index):
        addr = reg.addr[index]
        addr0 = addr + reg.bases[0]
        if addr0 in self.addr_space:
            self.addr_space[addr0].append((reg, index))
        else:
            self.addr_space[addr0] = [(reg, index)]
            for base in reg.bases[1:]:
                self.addr_space[base+addr] = self.addr_space[addr0]

def getDict(chipObj, bSaveUsed):
    dict = {}
    for key in chipObj.__dict__:
        if isinstance(chipObj.__dict__[key], RegHive):
            dict[key] = chipObj.__dict__[key].convertToDict(bSaveUsed)
    dict["regmapVersion"] = chipObj.__dict__["regmapVersion"]
    return dict


def save_RegsToFile(chipObj, projName, bSaveUsed, textFilePath = "regs.txt"):
    # global strDictRegs
    dictRegs = {}
    if os.path.exists(textFilePath) is True:
        file = open(textFilePath, "r+")
        strDictRegs = file.read()
        if strDictRegs is not None and len(strDictRegs) != 0:
            dictRegs = json.loads(strDictRegs)
        file.close()
    file = open(textFilePath, "w+")

    # folder = os.path.split(os.path.realpath(__file__))[0]
    dictRegs[projName] = getDict(chipObj, bSaveUsed)
    str = json.dumps(dictRegs)
    file.write(str)
    file.close()

def saveAllRegsToFile(chipObj, projName, textFilePath = "regs.txt"):
    save_RegsToFile(chipObj, projName, False, textFilePath = textFilePath)

def saveUsedRegsToFile(chipObj, projName, textFilePath = "regs.txt"):
    save_RegsToFile(chipObj, projName, True, textFilePath = textFilePath)

def getProjectName(filePath):
    (path, fileName) = os.path.split(filePath)
    (name, ext) = os.path.splitext(fileName)
    return name

def getGroupHive(dictRegs):
    group_hive = dict()
    hive = dictRegs
    avail_set = set(hive)
    for name in hive:
        if not name in avail_set: continue
        for number in re_number.finditer(name):
            prefix = name[:number.start(1)]
            postfix = name[number.end(1):]
            n = number.group(1)
            pattern = re.compile("^" + prefix + '(\d+)' + postfix + "$")
            n_found = 0
            found_set = dict()
            for other_name in avail_set:
                other_result = pattern.search(other_name)
                if other_result:
                    n_found = n_found + 1
                    found_set[other_result.group(1)] = other_name
            if n_found < 3: continue  # minimum 3 registers

            avail_set.difference_update(found_set.values())

            reglist = {int(x): hive[prefix + x + postfix] for x in found_set}
            # new name
            if prefix[-1] == "_":
                if postfix == "" or postfix[0] == "_": prefix = prefix[:-1]
            newname = prefix + postfix
            groupreg = ArrayReg(newname, reglist)
            group_hive[newname] = groupreg
    return group_hive


def load_hive_fromDict(name, dictHive, mdio_rd, mdio_wr):
    dict = {}
    laneCount = dictHive["laneNumber"]
    dictRegs = dictHive['regs']
    regs = {}
    for regName in dictRegs:
        dictRegOne = dictRegs[regName]
        reg = Reg(regName, None, None, None, None, None, None, dictRegOne['bases'])
        reg.RO = dictRegOne['RO']
        reg.width = dictRegOne['width']
        reg.reg_lsb = dictRegOne['reg_lsb']
        reg.addr = dictRegOne['addr']
        reg.lsb = dictRegOne['lsb']
        reg.def_value = dictRegOne['def_value']
        reg.parts = dictRegOne['parts']
        reg.bases = dictRegOne['bases']
        reg.sign = dictRegOne['sign']
        regs[regName] = reg
    group_regs = getGroupHive(regs)
    dict['regs'] = regs
    dict['group_regs'] = group_regs
    dict['lanes'] = laneCount
    regHive = RegHive(name, dict, mdio_rd, mdio_wr, True)
    return regHive

def load_Regs(projName, chip_obj=None, rd_func=None, wr_func=None, textFilePath = "regs.txt"):
    if os.path.exists(textFilePath) is  False:
        return None
    # global strDictRegs
    if rd_func is None:
        rd_func = getattr(chip_obj, "MdioRd", None)
        if rd_func is None:
            raise Exception("Can not find %s.MdioRd()" % chip_obj)
    if wr_func is None:
        wr_func = getattr(chip_obj, "MdioWr", None)
        if wr_func is None:
            raise Exception("Can not find %s.MdioWr()" % chip_obj)
    # Use specific R/W function
    if not callable(rd_func):
        raise Exception("rd_func must be callable")
    if not callable(wr_func):
        raise Exception("wr_func must be callable")

    file = open(textFilePath, "r+")
    strDictRegs = file.read()
    dictRegs = None
    if strDictRegs is not None and len(strDictRegs) != 0:
        dictRegs = json.loads(strDictRegs)
    else:
        file.close()
        return None
    if projName in dictRegs.keys():
        dict = dictRegs[projName]
        for group_name in dict:
            if group_name == "regmapVersion":
                setattr(chip_obj, group_name, dict[group_name])
            else:
                hive = load_hive_fromDict(group_name, dict[group_name], rd_func, wr_func)
                setattr(chip_obj, group_name, hive)
    file.close()

#------------------------------------------------------------------------------------------


def getRegisterDicts(dictRegisters, strPathRegisterMap):
    dictGroups = {}
    groups = load_regmap(strPathRegisterMap, None, None, None, dictRegisters)

    for gname in groups:
        if gname[0] == '_': continue
        regs = dict()
        group_regs = dict()
        for g in groups[gname].sheets:
            regs.update(g.regs)
            group_regs.update(g.group_regs)
        for d in groups[gname].dup:
            # if d in regs:
            regs[d] = DupReg(d, groups[gname].dup[d], groups[gname].dup_list[d])
            for i in range(len(groups[gname].dup[d])):
                # for s in hive_desc.dup[d]:
                # Add a longer name for each duplication
                s_index = groups[gname].sheet_index[groups[gname].dup[d][i]]
                new_name = groups[gname].dup_list[d][i]
                regs[new_name] = groups[gname].sheets[s_index].regs[d]
        dictSingle = {}
        dictSingle['regs'] = {}
        dictSingle['laneNumber'] = groups[gname].lanes
        for name in regs:
            dictRegOne = regs[name].convertToDict(False)
            if dictRegOne is not None:
                dictSingle['regs'][name] = dictRegOne

        dictGroups[gname] = dictSingle
    return dictGroups


def parseRegisterName(strRegister):
    strRegister = strRegister.strip()
    nameList = strRegister.split(".")
    if len(nameList) != 2:
        return None
    groupName = nameList[0]
    regName = nameList[1]
    index =  nameList[0].find("[")
    if index > 0:
        groupName = groupName[:index]
    index = nameList[1].find("[")
    if index > 0:
        regName = regName[:index]
    return groupName, regName

def addRegistersToTextFile(dictRegister, strPathRegisterMap, projName, strPathText = "regs.txt"):
    filterDict = {}
    dictRegs = {}
    if os.path.exists(strPathText) is True:
        file = open(strPathText, "r+")
        strDictRegs = file.read()
        if len(strDictRegs) != 0:
            dictRegs = json.loads(strDictRegs)
        file.close()
    file = open(strPathText, "w+")

    groups = getRegisterDicts(dictRegister, strPathRegisterMap)
    if dictRegs.has_key(projName) is False:
        dictRegs[projName] = groups
    else:
        for name in groups.keys():
            if dictRegs[projName].has_key(name) is True:
                dictRegs[projName][name]['regs'].update(groups[name]['regs'])
            else:
                dictRegs[projName][name] = groups[name];

    str = json.dumps(dictRegs)
    file.write(str)
    file.close()


def addRegisterToText(strRegister, strPathRegisterMap, projName, strPathText = "regs.txt"):
    filterDict = {}
    regName = parseRegisterName(strRegister)
    if regName is None:
        print "Format of register name error!"
        return
    filterDict[regName[0]] = [regName[1],]
    addRegistersToTextFile(filterDict, strPathRegisterMap, projName, strPathText)


def addRegistersToTextByFile(strPathConfig , strPathRegisterMap, projName, strPathText = "regs.txt"):
    if os.path.exists(strPathConfig) is False:
        print "config file is note exist"
        return
    filterDict = {}
    file = open(strPathConfig, "r+")
    strLine = file.readline()
    strLine = strLine.strip()
    lineNum = 1
    while(len(strLine) > 0):
        regName = parseRegisterName(strLine)
        if regName is None:
            print ("The format of regName in Line:%d error!"%lineNum)
        else:
            if filterDict.has_key(regName[0]) is not True:
                filterDict[regName[0]] = []
            filterDict[regName[0]].append(regName[1])
        strLine = file.readline()
        strLine = strLine.strip()
        lineNum += 1
    file.close()
    addRegistersToTextFile(filterDict, strPathRegisterMap, projName, strPathText)


def packagingFiles(strPathConfig):
    pass
#------------------------------------------------------------------------------------------


def load_regmap(filename, chip_obj=None, rd_func=None, wr_func=None, filterDict=None):
    # load_Regs(filename, chip_obj, rd_func, wr_func)
    # return
    print(filename)
    try:
        if os.path.exists(filename) is False:
            projName = getProjectName(filename)
            load_Regs(projName, chip_obj, rd_func, wr_func)
            return
    except Exception:
        pass
    
    w=xlrd.open_workbook(filename)
    # Find index table
    sn = w.sheet_names()
    # get version
    strVersion = ""
    if version_sheet_name in sn:
        version_sheet = w.sheet_by_name(version_sheet_name)
        for l in range(version_sheet.nrows):
            row = version_sheet.row(l)
            name = row[0].value
            if name == "version":
                strVersion = str(row[1].value)
                break
    
    if index_sheet_name not in sn:
        raise LookupError("Can not find Index worksheet in register map")
   
    index_sheet = w.sheet_by_name(index_sheet_name)
    
    
    # Check index
    hive_dict = dict()
    groups = dict()
    rev_map = RevMap()

    for l in range(index_sheet.nrows):
        lineno=l+1
        row = index_sheet.row(l)
        sheet_name = row[0].value
        if sheet_name == index_first_field and lineno==1:
            # title line
            continue
        if sheet_name is None: continue
        if sheet_name.strip() == "": continue
        group_names = row[3].value
        lanes = int(row[1].value)
        bases = row[2].value
        base = [int(v, 16) for v in bases.split(',')]
       
        # Sanity check
        if sheet_name not in sn:
            raise LookupError("Can not find worksheet %s on row %d of index"
                    % (sheet_name, lineno))
        if sheet_name in hive_dict.keys():
            raise LookupError("Duplicate sheet %s on row %d"
                    % (sheet_name, lineno))
        if len(base) != lanes:
            raise LookupError("Incorrect number of bases '%s' on row %d"
                    % (bases, lineno))
        
        sheet = w.sheet_by_name(sheet_name)

        sheet_name = sheet_name.strip().encode("ascii", "ignore")
        #print str(sheet_name)
        if filterDict is None:
            hive = load_hive(sheet, sheet_name, base, rev_map)
            H = SheetDescriptor(hive[0], hive[1], sheet_name, base)
        # split group names
        group_names=group_names.encode("ascii", "ignore")
        group_names = group_names.split(',')
        # Append a group for each sheet, if it's different
        group_from_sheet = sheet_transform(sheet_name)
        if group_from_sheet not in group_names:
            group_names.append(group_from_sheet)
        
        #-----------------------------------------------------
        listRegsFilter = []
        if filterDict is not None:
            for name in group_names:
                if filterDict.has_key(name) is False:
                    group_names.remove(name)
                else:
                    listRegsFilter.extend(filterDict[name])
            if len(group_names)== 0 or len(listRegsFilter) == 0:
                continue
        
        if filterDict is not None:
            hive = load_hive(sheet, sheet_name, base, rev_map, listRegsFilter)
            H = SheetDescriptor(hive[0], hive[1], sheet_name, base)
        #------------------------------------------------------
	#print str(group_names)	
        for g in group_names:
            #print g
            if g.strip()=="":
                raise LookupError("Empty group name in '%s'" % (group_names))
            if g not in groups.keys():
                groups[g]=HiveDescriptor()
            #print lanes
            #print H
            groups[g].add_sheet(H, lanes, g)
            
    # Check clash of namespace
    for g in groups:
        groups[g].check_dup()
  
    if chip_obj is not None:
        # Inject into chip object
        inject_reg(groups, chip_obj, rd_func, wr_func)
        setattr(chip_obj, "regmapVersion", strVersion)
    groups["_revmap"] = rev_map
    return groups

def inject_reg(groups, chip_obj, rd_func=None, wr_func=None):
    if rd_func is None:
        rd_func = getattr(chip_obj, "MdioRd", None)
        if rd_func is None:
            raise Exception("Can not find %s.MdioRd()" % chip_obj)
    if wr_func is None:
        wr_func = getattr(chip_obj, "MdioWr", None)
        if wr_func is None:
            raise Exception("Can not find %s.MdioWr()" % chip_obj)
    # Use specific R/W function
    if not callable(rd_func):
        raise Exception("rd_func must be callable")
    if not callable(wr_func):
        raise Exception("wr_func must be callable")
    #print 'groups', groups
    for gname in groups:
        #print 'gname', gname
        #print 'groups[gname]', groups[gname]
        if gname[0] == '_': continue
        group_hive = RegHive(gname, groups[gname], rd_func, wr_func)
        # install it
        #print 'group_hive', group_hive
        setattr(chip_obj, gname, group_hive)


class RegLane(object):
    def __init__(self, regs, group_regs, laneno, mdio_rd, mdio_wr, hive):
        object.__setattr__(self, "_mdio_rd", mdio_rd)
        object.__setattr__(self, "_mdio_wr", mdio_wr)
        object.__setattr__(self, "_laneno", laneno)
        object.__setattr__(self, "_regs", regs)
        object.__setattr__(self, "_group_regs", group_regs)
        #object.__setattr__(self, "_keys", keys)
        #object.__setattr__(self, "_gname", name)
        object.__setattr__(self, "_hive", hive)

    def convertToDict(self, bSaveUsed):
        dict = {}
        dictRegs = {}
        for name in self._regs:
            dictRegOne = self._regs[name].convertToDict(bSaveUsed)
            if dictRegOne is not None:
                dictRegs[name] = dictRegOne
        dict['regs'] = dictRegs
        return dict


    def info(self, name):
        if name not in self._regs:
            matchObj = re.match(r"(.+)\[(\d)\]", name)
            if matchObj and matchObj.group(1) in self._group_regs:
                return self._group_regs[matchObj.group(1)].info(int(matchObj.group(2)))
            # if name in self._group_regs:
            #     return self._group_regs[name]
            raise AttributeError("Group %s does not have register %s" %
                    (self._hive._gname, name))
        return self._regs[name].info()

    def __getattr__(self, name):
        if name not in self._regs:
            if name in self._group_regs:
                return self._group_regs[name]
            raise AttributeError("Group %s does not have register %s" %
                    (self._hive._gname, name))
        return self._regs[name].read(self._laneno, self._mdio_rd)

    def __setattr__(self, name, value):
        if name not in self._regs:
            if name[0] != '_':
                raise AttributeError("Group %s does not have register %s" %
                        (self._hive._gname, name))
            return super(RegLane, self).__setattr__(name, value)
        self._regs[name].write(value, self._laneno, self._mdio_rd, self._mdio_wr)

    def __dir__(self):
        return self._hive._keys

class RegAllLanes(object):
    def __init__(self, lanes):
        object.__setattr__(self, "_lanes", lanes)

    def __getattr__(self, name):
        return [l.__getattr__(name) for l in self._lanes]

    def __setattr__(self, name, value):
        for l in self._lanes:
            l.__setattr__(name, value)

    def __dir__(self):
        return self._lanes[0]._keys

class RegHive(object):
    def __init__(self, gname, hive_desc, mdio_rd, mdio_wr, byDict=False):
        regs = dict()
        group_regs = dict()

        if byDict is True:
            regs.update(hive_desc['regs'])
            group_regs.update(hive_desc['group_regs'])
            number_of_lanes = hive_desc['lanes']
        else:
            for g in hive_desc.sheets:
                regs.update(g.regs)
                group_regs.update(g.group_regs)
            for d in hive_desc.dup:
                #if d in regs:
                    regs[d] = DupReg(d, hive_desc.dup[d], hive_desc.dup_list[d])
                    for i in range(len(hive_desc.dup[d])):
                    #for s in hive_desc.dup[d]:
                        # Add a longer name for each duplication
                        s_index = hive_desc.sheet_index[hive_desc.dup[d][i]]
                        new_name = hive_desc.dup_list[d][i]
                        regs[new_name] = hive_desc.sheets[s_index].regs[d]
            number_of_lanes = hive_desc.lanes

        object.__setattr__(self, "_regs", regs)
        object.__setattr__(self, "_gname", gname)
        lanes = []
        for laneno in range(number_of_lanes):
            array_proxy = { name: ArrayRegProxy(group_regs[name],
                laneno, mdio_rd, mdio_wr) for name in group_regs }
            keys = regs.keys() + group_regs.keys()
            reg_lane = RegLane(
                    regs = regs, 
                    group_regs = array_proxy, 
                    laneno = laneno, 
                    mdio_rd = mdio_rd, 
                    mdio_wr = mdio_wr,
                    hive = self)
            lanes.append(reg_lane)
        object.__setattr__(self, "_lanes", lanes)
        object.__setattr__(self, "_alllanes", RegAllLanes(lanes))
        object.__setattr__(self, "_keys", keys)
        object.__setattr__(self, "_number_of_lanes", number_of_lanes)

    def convertToDict(self, bSaveUsed):
        dict = {}
        dict['regs'] = {}
        counts = len(self._lanes)
        for i in range(counts):
            dict['regs'].update(self._lanes[i].convertToDict(bSaveUsed)['regs'])
            dict['laneNumber'] = counts
        return dict

    def __dir__(self):
        return self._keys

    def __getitem__(self, index):
        if (index==-1):
            return self._alllanes
        else:
            return self._lanes[index]

    def __getattr__(self, name):
        if self._number_of_lanes==1:
            if name == "info":
                return self._lanes[0].info
            return self._lanes[0].__getattr__(name)
        else:
            raise RegException("Ambiguous multi-lane hive register access")

    def __setattr__(self, name, value):
        if self._number_of_lanes==1:
            return self._lanes[0].__setattr__(name, value)
        else:
            raise RegException("Ambiguous multi-lane hive register access")

    #def __delattr__(self, name):
    #    if self._number_of_lanes==1:
    #        return self._lanes[0].__delattr__(name, value)
    #    else:
    #        raise RegException("Ambiguous multi-lane hive register access")

class RegException(Exception):
    pass

def load_hive(sheet, sheet_name, bases, revmap, listRegsFilter = None):
    lineno = 0
    hive = dict()
    for l in range(sheet.nrows):
        lineno = l+1
        row = sheet.row(l)
        try:
            reg_addr = row[0].value
            #print str(reg_addr)
            if reg_addr is None: continue
            if row[0].ctype == xlrd.XL_CELL_EMPTY: continue
            
            if row[0].ctype != xlrd.XL_CELL_TEXT:
                print "Warning: Register address %g has number format, guessing!" % reg_addr
                reg_addr = "%g" % reg_addr
            
            reg_addr=reg_addr.strip()
            if reg_addr == sheet_first_field and lineno==1: continue   # title line
            if reg_addr == "": continue                         # empty line
            reg_name = row[1].value.strip()
            #print str(reg_name)
            desc = row[2].value.strip()
            default_value = row[3].value.strip()
            nob = int(row[4].value)
            rw = row[5].value
           
            #------------support new format
            strFormat = None
            if len(row) > 12:
                strFormat = row[12].value


            # Address
            addr_result = re_addr.search(reg_addr)
            if addr_result is None:
                raise RegException("Unrecognized address %s" % reg_addr)
            addr=int(addr_result.group(1), 16)
            msb=int(addr_result.group(2))
            lsb=msb if addr_result.lastindex==2 else int(addr_result.group(3))

            #print "%04x.%d:%d" % (addr, msb, lsb)
            # name and number of bits
            if re_RESERVED.search(reg_name): continue
            name_result = re_regname.search(reg_name)
            if name_result is None:
                raise RegException("Unrecognoized register name %s" % reg_name)
            name = name_result.group(1)
            if type(name)==unicode:
                name = name.encode('ascii', 'ignore')
            if name_result.lastindex>1:
                name_msb = int(name_result.group(2))
                name_lsb = name_msb if name_result.lastindex==2 else int(name_result.group(3))
            else:
                name_msb = None
                name_lsb = None

            if msb-lsb+1 != nob or (name_msb is not None and name_msb-name_lsb+1 != nob):
                if name_msb is None:
                    raise RegException("Inconsistent number of bits: %d:%d vs %d" % (msb, lsb, nob))
                else:
                    raise RegException("Inconsistent number of bits: %d:%d, %d:%d, %d" % (msb, lsb, name_msb, name_lsb, nob))
            if name_msb is None:
                if nob!=1:
                    print "Warning: Multi-bit register %s not marked, at line %d of sheet %s" % (name, lineno, sheet_name)
                name_msb = 0
                name_lsb = 0

            # -------check this reg need?
            if listRegsFilter is not None and name not in listRegsFilter:
                continue

            #sign
            sign = False
            if strFormat is not None:
                if re_sign.search(strFormat):
                    sign = True
            elif re_sign_1.search(desc) or re_sign_2.search(desc):
                sign = True
            
            # R/W
            # if rw != "R/W" and rw != "R":
            #     raise RegException("Unknown R/W status: %s" % rw)

            # Default value
            if default_value=="x" or default_value=="":
                default_value = 0
            else:
                def_value_result = re_def_value.search(default_value)
                if def_value_result is None:
                    raise RegException("Unrecognized default value %s" % default_value)
                def_value_nob = int(def_value_result.group(1))
                def_value_type = def_value_result.group(2)
                def_value_value = def_value_result.group(3)
                if def_value_nob!=nob:
                    raise RegException("Incorrect number of bits in default value %s" % default_value)
                def_value_base = 10
                if def_value_type == 'b':
                    def_value_base = 2
                elif def_value_type == 'h':
                    def_value_base = 16
                default_value = int(def_value_value, def_value_base)



            # Check multi-part register
            r = Reg(name, nob, name_lsb, addr, lsb, rw, default_value, bases, sign)
            r1, i = r.add_to_hive(hive)
            '''
            print "%s[%d:%d] @ %04x.%d:%d" % (name, name_msb, name_lsb, addr, msb, lsb)
            '''
            revmap.add(r1, i)
        except RegException as err:
            print "Error reading sheet %s, row %d: %s" % (sheet_name, lineno, format(err))
    # Cross check
    for r in hive.values():
        overlap=False
        msb=-1
        allset=set()
        for i in range(r.parts):
            thismsb=r.reg_lsb[i] + r.width[i]
            thisset = set(range(r.reg_lsb[i], thismsb))
            if not allset.isdisjoint(thisset): overlap=True
            allset = allset | thisset
            msb = max(msb, thismsb)
        if allset != set(range(0, msb)):
            print "Warning: Register %s does not cover all bits, in sheet %s" % (r.name, sheet_name)
        if overlap:
            print "Warning: Register %s has overlapped parts, in sheet %s" % (r.name, sheet_name)

    # Check overwrite
    ow_hive = dict()
    for owen_name in hive:
        owen_result = re_owen.search(owen_name)
        if not owen_result: continue
        new_name = re_owen.sub("", owen_name)
        ow_name = re_owen.sub("OW_", owen_name)
        if ow_name not in hive: continue
        ow = hive[ow_name]
        owen = hive[owen_name]
        if new_name in hive:
            print "Warning: overwrite register %s already exist in sheet %s, skipped" % (new_name, sheet_name)
            ow_hive[new_name] = OverwriteReg(new_name, owen, ow)
    hive.update(ow_hive)

    # Check group registers
    group_hive = dict()
    avail_set = set(hive)
    for name in hive:
        if not name in avail_set: continue
        for number in re_number.finditer(name):
            prefix = name[:number.start(1)]
            postfix = name[number.end(1):]
            n = number.group(1)
            pattern = re.compile('^'+ prefix + '(\d+)' + postfix + '$')
            n_found = 0
            found_set = dict()
            for other_name in avail_set:
                other_result=pattern.search(other_name)
                if other_result:
                    n_found = n_found+1
                    found_set[other_result.group(1)] = other_name
            if n_found<3: continue              # minimum 3 registers

            avail_set.difference_update(found_set.values())

            reglist = {int(x):hive[prefix+x+postfix] for x in found_set}
            # new name
            if prefix[-1]=="_":
                if postfix=="" or postfix[0]=="_": prefix=prefix[:-1]
            newname = prefix+postfix
            groupreg = ArrayReg(newname, reglist)
            group_hive[newname] = groupreg

    return (hive, group_hive)


def convert_script(filename, groups):
    rev_map = groups["_revmap"].addr_space
    with open(filename, 'r') as f:
        lineno = 0
        for line in f:
            lineno += 1
            result = re.match(r"([0-9a-fA-F]{4})\s+([0-9a-fA-F]{4})", line)
            if not result: continue
            addr = int(result.group(1), 16)
            value = int(result.group(2), 16)
            if addr not in rev_map:
                print "Address %04x not found in register map, at line %d" % (addr, lineno)
                continue
            register = rev_map[addr]
            header  = "%04x %04x:    " % (addr, value)
            header1 = "              "
            printed = False
            for r in register:
                index = r[1]
                rr = r[0]
                reg_def_value = rr.def_value[index]
                reg_lsb = rr.lsb[index]
                reg_reg_lsb = rr.reg_lsb[index]
                reg_width = rr.width[index]
                reg_name = rr.name
                reg_addr = rr.addr[index]
                reg_parts = rr.parts
                reg_base = rr.bases

                # check lane number
                if len(rr.bases)==1:
                    lane = ""
                else:
                    lane = "[%d]." % [reg_addr+b for b in rr.bases].index(addr)
                value_part = (value>>reg_lsb) & ((1<<reg_width)-1)
                if value_part != reg_def_value:
                    # Changed
                    if reg_parts!=1:
                        print "%s%s%s[%d:%d] = %d" % (header, lane, reg_name, 
                            reg_reg_lsb+reg_width-1, reg_reg_lsb, value_part)
                    else:
                        print "%s%s%s = %d" % (header, lane, reg_name,
                                value_part)
                    header = header1
                    printed = True
            if not printed:
                print "%sAll default value" % header

