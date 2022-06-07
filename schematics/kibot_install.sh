get_latest() {
    curl -s https://api.github.com/repos/$1/$2/releases/latest \
    | grep "browser_download_url.*deb" \
    | cut -d : -f 2,3 \
    | tr -d \" \
    | wget -qi - -P ./tmp/
}

echo "Downloading latest Kibot..."
get_latest INTI-CMNB KiBot
get_latest INTI-CMNB KiAuto
get_latest INTI-CMNB KiBoM
get_latest INTI-CMNB KiCost
get_latest INTI-CMNB pcbdraw
get_latest INTI-CMNB InteractiveHtmlBom

echo "Installing Kibot..."
sudo apt install -y ./tmp/*.deb

echo "Deleting temporary files..."
rm -rf ./tmp

echo "Installing pandoc"
sudo apt install -y pandoc

echo "Completed Kibot installation. You have:"
echo "- KiBot:   `kibot --version`"
echo "- KiAuto:  `pcbnew_do --version`"
echo "           `eeschema_do --version`"
# not sure how to check KiBoM version
echo "- KiCost:  `kicost --version`"
echo "- pcbdraw: `pcbdraw --version`"
# not sure how to check InteractiveHtmlBom version

echo "Done! If you get any errors, try uninstalling all packages and rerunning this script."