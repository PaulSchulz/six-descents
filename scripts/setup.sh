#!/bin/bash

# Pre System Boot (Boot without screen)
# Setup SSH
# - Add 'ssh' file in /boot partition
# - Add Wifi Credentials
#
# - System Boot

# Pre-System Setup (Boot with Screen and Keyboard)
# sudo su
# raspi-config
# - System Options
#   Wireless LAN
#     AU - Australia
#     SSID, Password
# - System Password
#   System Name
# - Display Options (2)
#   Screen Blanking (D4) - OFF
#   Composite (D6)- Enable 4Kp60 (V1)
# - Interface Options
#   SSH - Enable
# - Location Options
#   Locales
#     on:  en_AU.UTF-8 UTF-8
#     off: en_GB.UTF-8 UTF-8
#     system default: en_AU_UTF-8
#   Timezone
#     Australia/Sydney
#   Keyboard
#     Generic (104) - (Other) English(US) English(US)
#     AltGr Key: Default for this keyboard layout
#     Compose Key: No compose key
#   WAN Country - AU (Set during Wireless LAN Setup)
# - Advanced Options
#   Expand Filesystem (at next reboot)
#   HDMI / Composite - Enable 4k60
# - Update
#
# Reboot
# Record IP Address and test network connectivity
# HOSTNAME=???
# ping $HOSTNAME
# ssh pi@HOSTNAME

##############################################################################
# Packages
sudo apt update
sudo apt upgrade -y

# Development / Maintenance
sudo apt install -y git
sudo apt install -y build-essential
sudo apt install -y emacs
sudo apt install -y curl

##############################################################################
# Copy SSH key to Github
## For development
# ssh-keygen
# Copy ssh key to GITHUB
#  cat ~/.ssh/id_rsa.pub

mkdir -p ~/Documents/git
cd ~/Documents/git
# git clone https://github.com:PaulSchulz/six-descents.git
git clone git@github.com:PaulSchulz/six-descents.git
cd six-descents

##############################################################################
# NodeRED installation
# mkdir scripts
# curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered \
    #     > scripts/update-nodejs-and-nodered
# chmod 775 scripts/update-nodejs-and-nodered
./scripts/update-nodejs-and-nodered --confirm-install --confirm-pi --restart

sudo systemctl enable nodered.service

# Configure node-red, create a new settings file
# node-red admin init

# cd ~/.node-red/
#npm install @node-red-contrib-themes/theme-collection

# NodeRED additional package
#  cd ~/.node-red
#   npm i --unsafe-perm node-red-node-sqlite
#
# If upgrade of NodeRED
#  cd ~/.node-red
#  npm rebuild

# Add additional node-red packages

##############################################################################
# Build Binaries
sudo apt install -y libsqlite3-dev
sudo apt install -y libsdl2-dev
sudo apt install -y libsdl2-ttf-dev
make

##############################################################################
# SQLite Database
DATA=~/Documents/git/six-descents-data
mkdir -p $DATA
cd $DATA

# Create database
# See notes.org

##############################################################################
# Scripts

# Clear console
# printf "\033c" > /dev/console

# Build Binaries
# make

# Crontab - Add crontab file
# cat config/crontab | crontab -
