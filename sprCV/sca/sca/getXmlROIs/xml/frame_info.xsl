<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:param name="frame_number" />

  <xsl:template match="/">
    <frame_info>
      <frame_id>
        <xsl:value-of select="$frame_number" />
      </frame_id>
      <xsl:for-each select="//frame[@no=$frame_number]">
        <roi>
          <profile_type>
            <xsl:value-of select="../@type"/>
          </profile_type>
          <person_id>
            <xsl:value-of select="../../@id"/>
          </person_id>
          <behaviour_type>
            <xsl:value-of select="../../../@type"/>
          </behaviour_type>
          <person_description>
            <xsl:value-of select="../../@description"/>
          </person_description>
          <bb_ul_x>
            <xsl:value-of select="bb/@ul_x"/>
          </bb_ul_x>
          <bb_ul_y>
            <xsl:value-of select="bb/@ul_y"/>
          </bb_ul_y>
          <bb_lr_x>
            <xsl:value-of select="bb/@lr_x"/>
          </bb_lr_x>
          <bb_lr_y>
            <xsl:value-of select="bb/@lr_y"/>
          </bb_lr_y>
          <bb_or>
            <xsl:value-of select="bb/@or"/>
          </bb_or>
          <bb_gp_x>
            <xsl:value-of select="bb/@gp_x"/>
          </bb_gp_x>
          <bb_gp_y>
            <xsl:value-of select="bb/@gp_y"/>
          </bb_gp_y>
          <bb_gp_or>
            <xsl:value-of select="bb/@gp_or"/>
          </bb_gp_or>
        </roi>
      </xsl:for-each>
    </frame_info>
  </xsl:template>
</xsl:stylesheet>
