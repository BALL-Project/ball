find  ~/Desktop/BALLView-1.1.pkg -name ".DS_Store" -delete
hdiutil create -srcfolder /Users/oliver/Desktop/BALLView-1.1.pkg ./BALLView-Installer-1.1.dmg
hdiutil internet-enable -yes ./BALLView-Installer-1.1.dmg
