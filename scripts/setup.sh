#!/bin/bash

# System Setup
# raspi-config
# - Wifi to AU
# - Wifi network credentials
# - Locale AU and AU utf8, set default to en_AU.UTF-8
# - Timezone
# - Keyboard to US

##############################################################################
# Packages
sudo apt update
sudo apt upgrade -y

sudo apt install git
sudo apt install emacs
sudo apt install build-essential git curl

##############################################################################
# Copy SSH key to Github
mkdir -p ~/Documents/git
cd ~/Documents/git
git clone git@github.com:PaulSchulz/six-descents.git
cd six-decents

##############################################################################
# NodeRED installation
mkdir scripts
curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered \
     > scripts/update-nodejs-and-nodered
chmod 775 scripts/update-nodejs-and-nodered
./scripts/update-nodejs-and-nodered --confirm-install --confirm-pi --restart

sudo systemctl enable nodered.service

# node-red admin init

# NodeRED additional package
#  cd ~/.node-red
#   npm i --unsafe-perm node-red-node-sqlite
#
# If upgrade of NodeRED
#  cd ~/.node-red
#  npm rebuild

##############################################################################
# Change Console Font
#
# sudo apt install -y xterm-terminus
# sudo apt install -y bdf2psf
##############################################################################
# SQLite Software
# cd ~/Documents/git/six-descents/
sudo apt install -y libsqlite3-dev

##############################################################################
# SQLite Database
DATA=~/Documents/git/six-descents-data
mkdir -p $DATA
cd $DATA

# Create database
# See notes.org

##############################################################################
# Scripts
# sudo bash -c "watch src/display-events > /dev/console"

# Clear console
# printf "\033c" > /dev/console

# Set consolt font to bold
# tput bold > /dev/console

# Output with figlet
# ./src/display-events | figlet -f small -w 200 > /dev/console

# Change default console font
# sudo dpkg-reconfigure console-setup
## utf-8
## Latin1 and Latin5
## TerminusBold
## 16x32

# Crontab - Add crontab file
# cat config/crontab | crontab -
