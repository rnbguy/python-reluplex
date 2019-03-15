git clone https://github.com/guykatzz/ReluplexCav2017
cd ReluplexCav2017/glpk-4.60
./configure --prefix=`pwd`/installed
make
make install

cd -
python setup.py install --user
