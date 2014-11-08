#ifndef SERVERSTRS_H
#define SERVERSTRS_H

static const std::string badRequestResponse = "HTTP/1.0 400 Bad Request\n"
        "Content-type: text/html\n"
        "\n"
        "<html>\n"
        " <body>\n"
        "  <h1>Bad Request</h1>\n"
        "  <p>This server did not understand your request.</p>\n"
        " </body>\n"
        "</html>\n";

static const std::string badMethodResponseTemplate =
        "HTTP/1.0 501 Method Not Implemented\n"
        "Content-type: text/html\n"
        "\n"
        "<html>\n"
        " <body>\n"
        "  <h1>Method Not Implemented</h1>\n"
        "  <p>The method %s is not implemented by this server.</p>\n"
        " </body>\n"
        "</html>\n";

static const std::string pageStart =
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\"\n\"http://www.w3.org/TR/html4/strict.dtd\">\n"
        "<html>\n"
        "\t<head>\n"
        "\t<title>Q-Team Web Server</title>\n"
        "\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
        "\t</head>\n"
        "\t<body>\n"
        "\t\t<pre>\n";

static const std::string pageEnd =
        "\t\t</pre>\n"
        "\t</body>\n"
        "</html>\n\n";

#endif // SERVERSTRS_H
