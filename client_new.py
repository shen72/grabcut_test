#!/usr/bin/env python

from grabcut_test_pb2 import *
from GrabcutTestClient import *

import os, sys, cv

t_list = [0.95, 0.9, 0.8, 0.7, 0.6, 0.5]

def ext_num(fname):
  s = ''
  for ch in fname:
    if str.isdigit(ch):
      s += ch
  return long(s)

def load_list(list_name, dir_prefix = '.'):
  ret = dict()
  for fname in open(os.path.join(dir_prefix, list_name)):
    fname = fname.strip()
    img = cv.LoadImage(os.path.join(dir_prefix, fname), 0)
    img_id = ext_num(fname);
    ret[img_id] = ImageMask(image_id = img_id,
        image_mask = img.tostring()).SerializeToString()
  
  return ret

mask_dir = sys.argv[1]
bench_dir = sys.argv[2]

masklist = load_list('list.txt', mask_dir)
benchlist = load_list('list.txt', bench_dir)

client = GrabCutTestClient(9876)

prec = 0.0
rec = 0.0

rlist = list()

valid_num = 0

for img_id in benchlist:
  if img_id in masklist:
    response_proto = client.JudgeOne(masklist[img_id], benchlist[img_id])
    response = TestResponse.FromString(response_proto)
    if response.code == TestResponse.OK:
      prec = response.precision
      rec = response.recall
      #if rec > 0.5:
      rlist.append(prec)
      valid_num += 1

plist = list()
for thres in t_list:
  plist.append((thres, float(sum(map(lambda x: int(x >= thres), rlist))) / valid_num))

#fscore = prec * rec / (0.5 * prec + rec)
print '# of Valid samples:', valid_num,
print  'Precision:', plist
