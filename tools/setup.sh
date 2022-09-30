# a simple setup script

cd ~
mkdir esp
cd ~/esp
#git clone https://github.com/qt1/esp-idf.git -b ibn-20191007-mute_ser2_spam --recursive

#tested with espressif/release/v3.3
#git clone https://github.com/qt1/esp-idf.git -b ibn-20191007-mute_ser2_spam --recursive

git clone https://github.com/espressif/esp-idf.git -b release/v3.3
cd  ~/esp/esp-idf

#make the modules actuall pulled
git submodule update --init --recursive --remote
git submodule update --recursive

git remote add espressif https://github.com/espressif/esp-idf.git

cd ~/esp
git clone https://baruch23@bitbucket.org/ibn-labs/rmt_sender.git --recursive

#this causes colision with esp-idf
rm ~/esp/rmt_sender/components/arduino/libraries/AzureIoT/*

#sometimes also
git submodule update --init
#some suggest to delete the submodule befor update


cd ~/esp
wget https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
tar -xzf xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz

#see also https://docs.espressif.com/projects/esp-idf/en/stable/get-started/linux-setup.html