<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="document">
    <xsl:variable name="lcletters">abcdefghijklmnopqrstuvwxyz</xsl:variable>
    <xsl:variable name="ucletters">ABCDEFGHIJKLMNOPQRSTUVWXYZ</xsl:variable>
    <html xmlns="http://www.w3.org/1999/xhtml">
        <head>
            <title></title>
            <meta http-equiv="Content-Type" content="text/xml;charset=utf-8" />
            <meta name="ocr-id" content="abbyy"/>
            <meta name="ocr-recognized" content="words text"/>
        </head>
    <body>
        <xsl:for-each select="page">
            <xsl:variable name="pagewidth"><xsl:value-of select="@width" /></xsl:variable>                
            <xsl:variable name="pageheight"><xsl:value-of select="@height" /></xsl:variable>
            <xsl:variable name="pageId"><xsl:number from="/" level="any" count="page" /></xsl:variable>
            
            <div class="ocr_page" id="page_{$pageId}" title="bbox 0 0 {$pagewidth} {$pageheight}">
            <xsl:for-each select="block[@blockType='Text']">
                <xsl:variable name="x0"><xsl:value-of select="@l" /></xsl:variable>                
                <xsl:variable name="y0"><xsl:value-of select="@t" /></xsl:variable>
                <xsl:variable name="x1"><xsl:value-of select="@r" /></xsl:variable>                
                <xsl:variable name="y1"><xsl:value-of select="@b" /></xsl:variable>

                <div class="ocr_carea" title="bbox {$x0} {$y0} {$x1} {$y1}">
                <xsl:for-each select="par">
                    <p class='ocr_par'>
                    <xsl:for-each select="line">
                        <xsl:variable name="lineId"><xsl:number from="/" level="any" count="line" /></xsl:variable>
                        <xsl:variable name="lx0"><xsl:value-of select="@l" /></xsl:variable>                
                        <xsl:variable name="ly0"><xsl:value-of select="@t" /></xsl:variable>
                        <xsl:variable name="lx1"><xsl:value-of select="@r" /></xsl:variable>                
                        <xsl:variable name="ly1"><xsl:value-of select="@b" /></xsl:variable>
                        <span class="ocr_line" id="line_{$lineId}" title="bbox {$lx0} {$ly0} {$lx1} {$ly1}">
                            <!--<xsl:value-of select="." />-->
                            <xsl:for-each select="charParams">
                                <xsl:variable name="wordId"><xsl:number from="/" level="any" count="charParams" /></xsl:variable>
                                <xsl:variable name="wx0"><xsl:value-of select="@l" /></xsl:variable>                
                                <xsl:variable name="wy0"><xsl:value-of select="@t" /></xsl:variable>
                                <xsl:variable name="wx1"><xsl:value-of select="@r" /></xsl:variable>                
                                <xsl:variable name="wy1"><xsl:value-of select="@b" /></xsl:variable>
                                <span class="ocr_word" id="word_{$wordId}" title="bbox {$wx0} {$wy0} {$wx1} {$wy1}"><span class='ocrx_word' id='xword_{$wordId}' title="x_wconf 0"><xsl:value-of select="translate(.,$ucletters,$lcletters)"></xsl:value-of></span></span><xsl:text> </xsl:text>
                            </xsl:for-each>
                            <!--<xsl:variable name="xBoxes">
                                <xsl:value-of separator=" " select="(*[@l]|*[@t]|*[@r]|*[@b])[text()]" />
                            </xsl:variable>
                            <span class="ocr_cinfo" title="x_boxes {$xBoxes}"></span>-->
                        </span>  
                    </xsl:for-each>
                        <!--For charparams outside of lines-->
                        <xsl:for-each select="charParams">
                            <xsl:variable name="wordId"><xsl:number from="/" level="any" count="charParams" /></xsl:variable>
                            <xsl:variable name="wx0"><xsl:value-of select="@l" /></xsl:variable>                
                            <xsl:variable name="wy0"><xsl:value-of select="@t" /></xsl:variable>
                            <xsl:variable name="wx1"><xsl:value-of select="@r" /></xsl:variable>                
                            <xsl:variable name="wy1"><xsl:value-of select="@b" /></xsl:variable>
                            <span class="ocr_word" id="word_{$wordId}" title="bbox {$wx0} {$wy0} {$wx1} {$wy1}"><span class='ocrx_word' id='xword_{$wordId}' title="x_wconf 0"><xsl:value-of select="translate(.,$ucletters,$lcletters)"></xsl:value-of></span></span><xsl:text> </xsl:text>
                        </xsl:for-each>
                    </p>
                </xsl:for-each>
                </div>
            </xsl:for-each>
            </div>
        </xsl:for-each>
    </body>
    </html>
</xsl:template>

</xsl:stylesheet>

