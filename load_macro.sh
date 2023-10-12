#!/bin/bash
/opt/root/bin/root -l -x -e '.L analisi.C' -e 'analisi obj; obj.Loop();' 

