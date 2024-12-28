#!/bin/bash

cp "./libPcntQSign.so" "/opt/QQ/resources/app"
cp "./loadSign.js" "/opt/QQ/resources/app"

jq '.main = "./loadSign.js"' "/opt/QQ/resources/app/package.json" > "./package.json"
mv "./package.json" "/opt/QQ/resources/app/package.json"

xvfb-run qq --enable-logging --no-sandbox