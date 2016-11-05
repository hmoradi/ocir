__author__ = 'donny'

import StringIO
import os
import string


heading = "<h1>" + "OCIR Results" + "</h1>"

header = "<!DOCTYPE html> \n" \
         "<html> \n" \
         "<script type='text/javascript'> \n" \
         "function newWin(url,name, width, height) { \n" \
         "window.open(url,name,'scrollbars=yes,resizable=yes, width=' + width + ',height='+height); \n" \
         "}</script> \n" \
         "    <body>\n"

footer = "    </body>\n" \
         "</html>"

exps = open('./data/expconfig.txt')
res = open('result.html', 'w')
res.write(header)

for line in exps.readlines():

    folderName = "./build/"+line.strip()+"/"            
    debugFolder = folderName+"debug/"
    histFolder = folderName+"hist/"
    histColFolder = folderName+"hist_col/"
    otsuFolder = folderName+"otsu/"
    step = 0
    count = 1
    
    for subroot, subsubFolders, subfiles in os.walk(debugFolder):
        imgs =  []
        for file in subfiles:
            if file[-3:] != 'png':
                continue
            imgs.append(int(file[:-4]))
        
        imgs = sorted(imgs)

        f = open(str(step)+'.html', 'w')
        for imgNumber in imgs:
            img = str(imgNumber)+".png"
            print img
            if count > step:
                f.write("</table>")
                f.write("</div>")
                f.write(footer)
                f.close()

                f = open(line.strip()+"_"+str(step)+'.html', 'w')
                f.write(header)
                f.write("<div style=\"float:left;\">\n")
                f.write("<table style=\"width:100%\">");
                f.write("<tr>")
                f.write("<th>")
                f.write("Frame Number")
                f.write("</th>")
                f.write("<th>")
                f.write("Original Frame")
                f.write("</th>")
                f.write("<th>")
                f.write("Otsu Thresholding")
                f.write("</th>")
                f.write("<th>")
                f.write("Histogram of All Pixels")
                f.write("</th>")
                f.write("<th>")
                f.write("Histogram of Columns")
                f.write("</th>")
                
                f.write("</tr>")
                step += 100
            
            f.write("<tr>")
            f.write("<th>")
            f.write(img[:-4])
            f.write("</th>")

            f.write("<th>")
            strImg = "<a><img src=\"{0}{1}\" width=\"128\" height=\"128\" /></a>\n".format(debugFolder, img)
            f.write(strImg)
            f.write("</th>")
            f.write("<th>")
            strImg = "<a><img src=\"{0}{1}\" width=\"128\" height=\"128\" /></a>\n".format(otsuFolder, img)
            f.write(strImg)
            f.write("</th>")
            f.write("<th>")
            strImg = "<a><img src=\"{0}{1}\" width=\"128\" height=\"128\" /></a>\n".format(histFolder, img)
            f.write(strImg)
            f.write("</th>")
            f.write("<th>")
            strImg = "<a><img src=\"{0}{1}\" width=\"128\" height=\"128\" /></a>\n".format(histColFolder, img)
            f.write(strImg)
            f.write("</th>")
            f.write("</tr>")
            
            count += 1
           
    res.write(line+"</br>")
    for i in range(0,step,100):
        res.write("<a href=\""+line.strip()+"_"+str(i)+".html\">"+str(i)+"</a></br>")
res.write(footer)
res.close()
