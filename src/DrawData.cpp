
#include "DrawData.h"

#include "lib/svglite_utils.h"

#include <cmath>
#include <fmt/ostream.h>
#include <string>
#include <vector>

namespace httpgd::dc
{
    inline void css_fill_or_none(std::ostream &os, color_t col)
    {
        int alpha = R_ALPHA(col);
        if (alpha == 0)
        {
            os << "fill: none;";
        }
        else
        {
            fmt::print(os, "fill: #{:02X}{:02X}{:02X};", R_RED(col), R_GREEN(col), R_BLUE(col));
            if (alpha != 255)
            {
                fmt::print(os, "fill-opacity: {:.2f};", alpha / 255.0);
            }
        }
    }

    inline void css_fill_or_omit(std::ostream &os, color_t col)
    {
        int alpha = R_ALPHA(col);
        if (alpha != 0)
        {
            fmt::print(os, "fill: #{:02X}{:02X}{:02X};", R_RED(col), R_GREEN(col), R_BLUE(col));
            if (alpha != 255)
            {
                fmt::print(os, "fill-opacity: {:.2f};", alpha / 255.0);
            }
        }
    }

    inline double scale_lty(int lty, double lwd)
    {
        // Don't rescale if lwd < 1
        // https://github.com/wch/r-source/blob/master/src/library/grDevices/src/cairo/cairoFns.c#L134
        return ((lwd > 1) ? lwd : 1) * (lty & 15);
    }
    inline void css_lineinfo(std::ostream &os, const LineInfo &line)
    {

        // 1 lwd = 1/96", but units in rest of document are 1/72"
        fmt::print(os, "stroke-width: {:.2f};", line.lwd / 96.0 * 72);

        // Default is "stroke: #000000;" as declared in <style>
        if (line.col != R_RGBA(0, 0, 0, 255))
        {
            int alpha = R_ALPHA(line.col);
            if (alpha == 0)
            {
                fmt::print(os, "stroke: none;");
            }
            else
            {
                fmt::print(os, "stroke: #{:02X}{:02X}{:02X};", R_RED(line.col), R_GREEN(line.col), R_BLUE(line.col));
                if (alpha != 255)
                {
                    fmt::print(os, "stroke-opacity: {:.2f};", alpha / 255.0);
                }
            }
        }

        // Set line pattern type
        int lty = line.lty;
        switch (lty)
        {
        case LTY_BLANK: // never called: blank lines never get to this point
        case LTY_SOLID: // default svg setting, so don't need to write out
            break;
        default:
            // For details
            // https://github.com/wch/r-source/blob/trunk/src/include/R_ext/GraphicsEngine.h#L337
            os << " stroke-dasharray: ";
            // First number
            fmt::print(os, "{:.2f}", scale_lty(lty, line.lwd));
            lty = lty >> 4;
            // Remaining numbers
            for (int i = 1; i < 8 && lty & 15; i++)
            {
                fmt::print(os, ", {:.2f}", scale_lty(lty, line.lwd));
                lty = lty >> 4;
            }
            os << ";";
            break;
        }

        // Set line end shape
        switch (line.lend)
        {
        case LineInfo::GC_ROUND_CAP: // declared to be default in <style>
            break;
        case LineInfo::GC_BUTT_CAP:
            os << "stroke-linecap: butt;";
            break;
        case LineInfo::GC_SQUARE_CAP:
            os << "stroke-linecap: square;";
            break;
        default:
            break;
        }

        // Set line join shape
        switch (line.ljoin)
        {
        case LineInfo::GC_ROUND_JOIN: // declared to be default in <style>
            break;
        case LineInfo::GC_BEVEL_JOIN:
            os << "stroke-linejoin: bevel;";
            break;
        case LineInfo::GC_MITRE_JOIN:
            os << "stroke-linejoin: miter;";
            if (std::abs(line.lmitre - 10.0) > 1e-3)
            { // 10 is declared to be the default in <style>
                fmt::print(os, "stroke-miterlimit: {:.2f};", line.lmitre);
            }
            break;
        default:
            break;
        }
    }

    void svg_clip_attr(std::ostream &os, clip_id_t id)
    {
        fmt::print(os, R""(clip-path="url(#c{:d})")"", id);
    }

    // DRAW CALL OBJECTS

    clip_id_t DrawCall::clip_id() const
    {
        return m_clip_id;
    }

    void DrawCall::clip_id(clip_id_t t_clip_id)
    {
        m_clip_id = t_clip_id;
    }

    void DrawCall::svg(std::ostream &os) const
    {
        os << "<!-- unknown draw call -->";
    }

    Text::Text(color_t t_col, vertex<double> t_pos, std::string &&t_str, double t_rot, double t_hadj, TextInfo &&t_text)
        : m_col(t_col), m_pos(t_pos), m_rot(t_rot), m_hadj(t_hadj), m_str(t_str), m_text(t_text)
    {
    }
    void Text::svg(std::ostream &os) const
    {
        // If we specify the clip path inside <image>, the "transform" also
        // affects the clip path, so we need to specify clip path at an outer level
        // (according to svglite)
        os << "<g ";
        svg_clip_attr(os, clip_id());
        os << "><text ";

        if (m_rot == 0.0)
        {
            fmt::print(os, R""(x="{:.2f}" y="{:.2f}" )"", m_pos.x, m_pos.y);
        }
        else
        {
            fmt::print(os, R""(transform="translate({:.2f},{:.2f}) rotate({:.2f})" )"", m_pos.x, m_pos.y, m_rot * -1.0);
        }

        if (m_hadj == 0.5)
        {
            os << R""(text-anchor="middle" )"";
        }
        else if (m_hadj == 1)
        {
            os << R""(text-anchor="end" )"";
        }

        os << "style=\"";
        fmt::print(os, "font-family: {};font-size: {:.2f}px;", m_text.font_family, m_text.fontsize);

        if (m_text.weight != 400)
        {
            if (m_text.weight == 700)
            {
                os << "font-weight: bold;";
            }
            else
            {
                fmt::print(os, "font-weight: {:.2f};", m_text.weight);
            }
        }
        if (m_text.italic)
        {
            os << "font-style: italic;";
        }
        if (m_col != (int)R_RGB(0, 0, 0))
        {
            css_fill_or_none(os, m_col);
        }
        if (m_text.features.length() > 0)
        {
            fmt::print(os, "font-feature-settings: {};", m_text.features);
        }
        os << "\"";
        if (m_text.txtwidth_px > 0)
        {
            fmt::print(os, R""( textLength="{:.2f}px" lengthAdjust="spacingAndGlyphs")"", m_text.txtwidth_px);
        }
        os << ">";
        write_xml_escaped(os, m_str);
        os << "</text></g>";
    }

    Circle::Circle(LineInfo &&t_line, color_t t_fill, vertex<double> t_pos, double t_radius)
        : m_line(t_line), m_fill(t_fill), m_pos(t_pos), m_radius(t_radius)
    {
    }
    void Circle::svg(std::ostream &os) const
    {
        os << "<circle ";
        svg_clip_attr(os, clip_id());
        fmt::print(os, R""( cx="{:.2f}" cy="{:.2f}" r="{:.2f}" )"", m_pos.x, m_pos.y, m_radius);

        os << "style=\"";
        css_lineinfo(os, m_line);
        css_fill_or_omit(os, m_fill);
        os << "\"/>";
    }

    Line::Line(LineInfo &&t_line, vertex<double> t_orig, vertex<double> t_dest)
        : m_line(t_line), m_orig(t_orig), m_dest(t_dest)
    {
    }
    void Line::svg(std::ostream &os) const
    {
        os << "<line ";
        svg_clip_attr(os, clip_id());
        fmt::print(os, R""( x1="{:.2f}" y1="{:.2f}" x2="{:.2f}" y2="{:.2f}" )"", m_orig.x, m_orig.y, m_dest.x, m_dest.y);

        os << "style=\"";
        css_lineinfo(os, m_line);
        os << "\"/>";
    }

    Rect::Rect(LineInfo &&t_line, color_t t_fill, rect<double> t_rect)
        : m_line(t_line), m_fill(t_fill), m_rect(t_rect)
    {
    }
    void Rect::svg(std::ostream &os) const
    {
        os << "<rect ";
        svg_clip_attr(os, clip_id());
        fmt::print(os, R""( x="{:.2f}" y="{:.2f}" width="{:.2f}" height="{:.2f}" )"",
                   m_rect.x,
                   m_rect.y,
                   m_rect.width,
                   m_rect.height);

        os << "style=\"";
        css_lineinfo(os, m_line);
        css_fill_or_omit(os, m_fill);
        os << "\"/>";
    }

    Polyline::Polyline(LineInfo &&t_line, std::vector<vertex<double>> &&t_points)
        : m_line(t_line), m_points(t_points)
    {
    }
    void Polyline::svg(std::ostream &os) const
    {
        os << "<polyline ";
        svg_clip_attr(os, clip_id());
        os << " points=\"";
        for (auto it = m_points.begin(); it != m_points.end(); ++it)
        {
            if (it != m_points.begin())
            {
                os << " ";
            }
            fmt::print(os, "{:.2f},{:.2f}", it->x, it->y);
        }
        os << "\" style=\"";
        css_lineinfo(os, m_line);
        os << "\"/>";
    }
    Polygon::Polygon(LineInfo &&t_line, color_t t_fill, std::vector<vertex<double>> &&t_points)
        : m_line(t_line), m_fill(t_fill), m_points(t_points)
    {
    }
    void Polygon::svg(std::ostream &os) const
    {
        os << "<polygon ";
        svg_clip_attr(os, clip_id());

        os << " points=\"";
        for (auto it = m_points.begin(); it != m_points.end(); ++it)
        {
            if (it != m_points.begin())
            {
                os << " ";
            }
            fmt::print(os, "{:.2f},{:.2f}", it->x, it->y);
        }
        os << "\" ";

        os << "style=\"";
        css_lineinfo(os, m_line);
        css_fill_or_omit(os, m_fill);
        os << "\" ";

        os << "/>";
    }
    Path::Path(LineInfo &&t_line, color_t t_fill, std::vector<vertex<double>> &&t_points, std::vector<int> &&t_nper, bool t_winding)
        : m_line(t_line), m_fill(t_fill), m_points(t_points), m_nper(t_nper), m_winding(t_winding)
    {
    }
    void Path::svg(std::ostream &os) const
    {
        os << "<path ";
        svg_clip_attr(os, clip_id());
        os << " d=\"";

        auto it_poly = m_nper.begin();
        std::size_t left = 0;
        for (auto it = m_points.begin(); it != m_points.end(); ++it)
        {
            if (left == 0)
            {
                left = *it_poly;
                ++it_poly;
                if (it != m_points.begin())
                {
                    os << "Z";
                }
                os << "M ";
            }
            else
            {
                --left;
                os << "L ";
            }
            fmt::print(os, "{:.2f},{:.2f}", it->x, it->y);
        }

        // Finish path data
        os << "\" style=\"";
        css_lineinfo(os, m_line);
        css_fill_or_omit(os, m_fill);
        os << "fill-rule: "
           << (m_winding ? "nonzero" : "evenodd")
           << ";\"/>";
    }

    Raster::Raster(std::vector<unsigned int> &&t_raster, vertex<int> t_wh,
               rect<double> t_rect,
               double t_rot,
               bool t_interpolate)
        : m_raster(t_raster), m_wh(t_wh), m_rect(t_rect), m_rot(t_rot), m_interpolate(t_interpolate)
    {
    }
    void Raster::svg(std::ostream &os) const
    {

        // If we specify the clip path inside <image>, the "transform" also
        // affects the clip path, so we need to specify clip path at an outer level
        // (according to svglite)
        os << "<g ";
        svg_clip_attr(os, clip_id());
        os << "><image ";
        fmt::print(os, R""( x="{:.2f}" y="{:.2f}" width="{:.2f}" height="{:.2f}" )"",
                   m_rect.x,
                   m_rect.y,
                   m_rect.width,
                   m_rect.height);
        os << R""(preserveAspectRatio="none" )"";
        if (!m_interpolate)
        {
            os << R""(image-rendering="pixelated" )"";
        }
        if (m_rot != 0)
        {
            fmt::print(os, R""(transform="rotate({:.2f},{:.2f},{:.2f})" )"", -1.0 * m_rot, m_rect.x, m_rect.y);
        }
        os << " xlink:href=\"data:image/png;base64,";
        os << raster_to_string(m_raster, m_wh.x, m_wh.y, m_rect.width, m_rect.height, m_interpolate);
        os << "\"/></g>";
    }

    Clip::Clip(clip_id_t t_id, rect<double> t_rect)
        : m_id(t_id), m_rect(t_rect)
    {
    }
    const double CLIP_EPSILON = 0.01;
    bool Clip::equals(rect<double> t_rect) const
    {
        return std::abs(t_rect.x - m_rect.x) < CLIP_EPSILON &&
               std::abs(t_rect.x - m_rect.x) < CLIP_EPSILON &&
               std::abs(t_rect.width - m_rect.width) < CLIP_EPSILON &&
               std::abs(t_rect.height - m_rect.height) < CLIP_EPSILON;
    }
    clip_id_t Clip::id() const
    {
        return m_id;
    }
    void Clip::svg_def(std::ostream &os) const
    {
        fmt::print(os, R""(<clipPath id="c{:d}"><rect x="{:.2f}" y="{:.2f}" width="{:.2f}" height="{:.2f}"/></clipPath>)"",
                   m_id,
                   m_rect.x,
                   m_rect.y,
                   m_rect.width,
                   m_rect.height);
    }

    Page::Page(page_id_t t_id, vertex<double> t_size)
        : m_id(t_id), m_size(t_size)
    {
        clip({0, 0, m_size.x, m_size.y});
    }

    page_id_t Page::id() const
    {
        return m_id;
    }

    vertex<double> Page::size() const
    {
        return m_size;
    }
    void Page::size(vertex<double> t_size)
    {
        m_size = t_size;
    }
    
    void Page::fill(color_t t_fill)
    {
        m_fill = t_fill;
    }

    void Page::clip(rect<double> t_rect)
    {
        const auto cps_count = m_cps.size();
        if (cps_count == 0 || !m_cps.back().equals(t_rect))
        {
            m_cps.emplace_back(Clip(cps_count, t_rect));
        }
    }

    void Page::put(std::shared_ptr<DrawCall> dc)
    {
        m_dcs.emplace_back(dc);
        dc->clip_id(m_cps.back().id());
    }

    void Page::clear()
    {
        m_dcs.clear();
        m_cps.clear();
        clip({0, 0, m_size.x, m_size.y});
    }
    void Page::svg(std::ostream &os, boost::optional<const std::string &> t_extra_css) const
    {
        os << R""(<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" class="httpgd" )"";
        fmt::print(os,
                   R""(width="{:.2f}" height="{:.2f}" viewBox="0 0 {:.2f} {:.2f}")"",
                   m_size.x, m_size.y, m_size.x, m_size.y);
        os << ">\n<defs>\n"
              "  <style type='text/css'><![CDATA[\n";
        if (t_extra_css)
        {
            os << *t_extra_css << "\n";
        }
        os << "    .httpgd line, .httpgd polyline, .httpgd polygon, .httpgd path, .httpgd rect, .httpgd circle {\n"
              "      fill: none;\n"
              "      stroke: #000000;\n"
              "      stroke-linecap: round;\n"
              "      stroke-linejoin: round;\n"
              "      stroke-miterlimit: 10.00;\n"
              "    }\n"
              "  ]]></style>\n";

        for (const auto &cp : m_cps)
        {
            cp.svg_def(os);
            os << "\n";
        }
        os << "</defs>\n";
        fmt::print(os, R""(<rect width="100%" height="100%" style="stroke: none; fill: #{:02X}{:02X}{:02X};"/>)"" "\n",
                   R_RED(m_fill), R_GREEN(m_fill), R_BLUE(m_fill));
        for (const auto &dc : m_dcs)
        {
            dc->svg(os);
            os << "\n";
        }
        os << "</svg>";
    }

} // namespace httpgd::dc