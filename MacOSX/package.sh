find  "$1" -name ".DS_Store" -delete
hdiutil create -srcfolder "$1" ./BALLView-Installer-1.3-RC1.dmg
hdiutil internet-enable -yes ./BALLView-Installer-1.3-RC1.dmg
