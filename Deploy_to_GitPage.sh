#!/bin/bash
rm -rf ./docs
cp -r ./Config_Website ./docs
git add ./docs
git commit -m "Deploy Website"
git push origin master