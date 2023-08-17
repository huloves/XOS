
echo "######## arch ########"
cloc arch --fullpath

echo "######## boot ########"
cloc boot --fullpath

echo "######## drivers ########"
cloc drivers --fullpath

echo "######## fs ########"
cloc fs --fullpath

echo "######## include ########"
cloc include --fullpath

echo "######## init ########"
cloc init --fullpath

echo "######## kernel ########"
cloc kernel --fullpath

echo "######## libs ########"
cloc lib --fullpath

echo "######## mm ########"
cloc mm --fullpath

echo "######## sum ########"
dirs="arch drivers fs hyp include init kernel libs mm usr"
cloc ${dirs} --fullpath

