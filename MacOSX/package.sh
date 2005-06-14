find  ~/Desktop/BALLView.pkg -name ".DS_Store" -delete
hdiutil create -srcfolder /Users/oliver/Desktop/BALLView.pkg ./BALLView-Installer.dmg
hdiutil internet-enable -yes ./BALLView-Installer.dmg
