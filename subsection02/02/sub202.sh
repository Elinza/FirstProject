#!/bin/bash
read -p "What file do you want to sort : " path
echo `sort -d -r $path` 
