<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:mods="http://www.loc.gov/mods/v3" xmlns="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" exclude-result-prefixes="mods">
    <xsl:output method="xml" indent="yes"/>
    <xsl:template match="*[not(node())]"/> <!-- strip empty elements -->
    <xsl:template match="/">
        <html>
            <head>
                <style type="text/css">
#modsRow {
width: 420px;
margin: 2px;
}
#modsLabel {
width: 100px;
font-weight:bold;
vertical-align:text-top;
text-align: right;
padding: 3px;
}
#modsValue {
vertical-align:text-top;
padding: 3px;
}
</style>
            </head>
            <body>
                <table>
                <xsl:if test="//mods:title/text() [normalize-space(.) ]">
                    <tr id="modsRow">
                        <td id="modsLabel">Title:</td><td id="modsValue"><xsl:value-of select="//mods:title"/>
                            <xsl:if test="//mods:subTitle/text() [normalize-space(.) ]"> : 
: <xsl:value-of select="//mods:subTitle"/></xsl:if>
                        </td>
                        </tr>
                </xsl:if>
                    <xsl:if test="//mods:dateCreated/text() [normalize-space(.) ] | //mods:dateIssued/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Date:</td><td id="modsValue"><xsl:value-of select="//mods:dateCreated"/><xsl:value-of select="//mods:dateIssued"/></td>
                        </tr>
                    </xsl:if> 
                    <xsl:for-each select="//mods:name">
                                <tr id="modsRow">
                                    <td id="modsLabel">Name:</td><td id="modsValue">
                                        <xsl:value-of select="mods:namePart"/>
                                        <br /><xsl:value-of select="mods:role/mods:roleTerm"/>                                   
                                    </td>
                                </tr>
                    </xsl:for-each>
                  
                <xsl:if test="//mods:abstract/text() [normalize-space(.) ]">
                    <tr id="modsRow">
                        <td id="modsLabel">Abstract:</td><td id="modsValue"><xsl:value-of select="//mods:abstract"/></td>
                    </tr>
                </xsl:if> 
                    <xsl:if test="//mods:identifier/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Identifier:</td><td id="modsValue"><xsl:value-of select="//mods:identifier"/></td>
                        </tr>
                    </xsl:if> 
                    <xsl:if test="//mods:identifier/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Physical Description</td>
                            <td id="modsValue"><ul>
                                <xsl:if test="//mods:extent/text() [normalize-space(.) ]"><li><xsl:value-of select="//mods:extent"/></li></xsl:if>
                                <xsl:if test="//mods:form/text() [normalize-space(.) ]"><li><xsl:value-of select="//mods:form"/></li></xsl:if>      
                                
                                <xsl:if test="//mods:note[not(@*)]/text() [normalize-space(.) ]"><li><xsl:value-of select="//mods:note[not(@*)]"/></li></xsl:if>
                                <xsl:if test="//mods:note[@type='physical']/text() [normalize-space(.) ]"><li><xsl:value-of select="//mods:note[@type='physical']"/></li></xsl:if>        
                                <xsl:if test="//mods:note[@type='details']/text() [normalize-space(.) ]"><li><xsl:value-of select="//mods:note[@type='details']"/></li></xsl:if>
                                <xsl:if test="//mods:scale/text() [normalize-space(.) ]"><li>Scale: <xsl:value-of select="//mods:scale"/></li></xsl:if>
                                <xsl:if test="//mods:coordinates/text() [normalize-space(.) ]"><li><xsl:value-of select="//mods:coordinates"/></li></xsl:if>
                                </ul>
                            </td>
                        </tr>
                    </xsl:if> 
                    <xsl:for-each select="/mods:mods/mods:subject[@authority='paro']/mods:topic/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Topic:</td><td id="modsValue">
                                <xsl:value-of select="."/>
                            </td>
                        </tr>
                    </xsl:for-each>
                    <xsl:for-each select="/mods:mods/mods:subject[@authority='lcsh']/mods:topic/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Topic:</td><td id="modsValue">
                                <xsl:value-of select="."/>
                            </td>
                        </tr>
                    </xsl:for-each>
                    <xsl:if test="//mods:continent/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Continent:</td><td id="modsValue"><xsl:value-of select="//mods:continent"/></td>
                        </tr>
                    </xsl:if>
                    <xsl:if test="//mods:country/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Country:</td><td id="modsValue"><xsl:value-of select="//mods:country"/></td>
                        </tr>
                    </xsl:if>
                    
                    <xsl:if test="//mods:province/text() [normalize-space(.) ]">
                    <xsl:for-each select="//mods:province">
                        <tr id="modsRow">
                            <td id="modsLabel">Province:</td><td id="modsValue">
                                <xsl:value-of select="."/>
                            </td>
                        </tr>
                    </xsl:for-each>
                    </xsl:if>
                    <xsl:if test="//mods:county/text() [normalize-space(.) ]">
                        <xsl:for-each select="//mods:county">
                            <tr id="modsRow">
                                <td id="modsLabel">County:</td><td id="modsValue">
                                    <xsl:value-of select="."/>
                                </td>
                            </tr>
                        </xsl:for-each>
                    </xsl:if>
                    <xsl:if test="//mods:region/text() [normalize-space(.) ]">
                        <xsl:for-each select="//mods:region">
                            <tr id="modsRow">
                                <td id="modsLabel">Region/Lot:</td><td id="modsValue">
                                    <xsl:value-of select="."/>
                                </td>
                            </tr>
                        </xsl:for-each>
                    </xsl:if> 
                    
                    <xsl:if test="//mods:city/text() [normalize-space(.) ]">
                        <xsl:for-each select="//mods:city">
                            <tr id="modsRow">
                                <td id="modsLabel">City:</td><td id="modsValue">
                                    <xsl:value-of select="."/>
                                </td>
                            </tr>
                        </xsl:for-each>
                    </xsl:if> 
                        <xsl:if test="//mods:citySection/text() [normalize-space(.) ]">
                            <tr id="modsRow">
                                <td id="modsLabel">City Section:</td><td id="modsValue"><xsl:value-of select="//mods:citySection"/></td>
                            </tr>
                        </xsl:if>                   
                    <xsl:if test="//mods:accessCondition[@type='useAndReproduction']/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Use and Reproduction:</td><td id="modsValue"><xsl:value-of select="//mods:accessCondition[@type='useAndReproduction']"/></td>
                        </tr>
                    </xsl:if>
                    <xsl:if test="//mods:accessCondition[@type='restrictionOnAccess']/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Restrictions on Access:</td><td id="modsValue"><xsl:value-of select="//mods:accessCondition[@type='restrictionOnAccess']"/></td>
                        </tr>
                    </xsl:if>
                    <xsl:if test="//mods:physicalLocation/text() [normalize-space(.) ]">
                        <tr id="modsRow">
                            <td id="modsLabel">Physical Location:</td><td id="modsValue"><xsl:value-of select="//mods:physicalLocation"/></td>
                        </tr>
                    </xsl:if>
                    
                </table>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
