
function WriteHeader(title)
{
    document.write("<table width='100%' border='0' cellpadding='0' cellspacing='0'><tr>\n");
    document.write("    <td class='HeaderCell' width='20'><div class='HeaderAppLogo'><img src='../Graphics/AppIcon.gif' width='20' height='20' border='0'></div></td>\n");
    document.write("    <td class='HeaderCell'><div class='HeaderTitle'><span style='letter-spacing: 1em;'>Meazure</span><span class='TM'>TM</span></div></td>\n");
    document.write("    <td class='HeaderCell' width='19'><div class='HeaderCoLogo'><img src='../Graphics/HeaderLogo.gif' width='19' height='20' border='0'></div></td>\n");
    document.write("</tr></table>\n");
    document.write("<div class='Page'>\n");
    document.write("    <div class='Title'>" + title + "</div>\n");
}

function WriteFooter()
{
    document.write("    </div>\n");
    document.write("    <hr width='95%'>\n");
    document.write("    <div class='Footer'>\n");
    document.write("    <div class='Copyright'>Copyright &copy; 2001, 2004, 2011 C Thing Software</div>\n");
    document.write("    <div class='Contact'>Contact <a href='mailto:meafeedback@cthing.com'>C Thing Software</a> or visit us at <a href='http://www.cthing.com' target='new'>www.cthing.com</a></div>\n");
    document.write("    <div class='FooterBottom'></div>\n");
    document.write("</div>\n");
}
