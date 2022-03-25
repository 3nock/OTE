#include "ParserMethods.h"


///
/// getting html body node...
///
GumboNode* getBody(GumboNode *node) {
    for(unsigned int i = 0; i < node->v.element.children.length; i++)
    {
        GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[i]);
        if(child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_BODY)
            return child;
    }
    return nullptr;
}

///
/// getting document title...
///
QString getTitle(QNetworkReply *reply) {
    QString title;
    QStack<GumboNode*> node_stack;
    GumboOutput *output = gumbo_parse(reply->readAll());
    node_stack.push(getBody(output->root));

    GumboNode *node;
    while(!node_stack.isEmpty())
    {
        node = node_stack.pop();
        if (node->type != GUMBO_NODE_ELEMENT)
            continue;

        if (node->v.element.tag == GUMBO_TAG_H1){
            GumboNode* title_text = static_cast<GumboNode*>(node->v.element.children.data[0]);
            title = QString::fromUtf8(title_text->v.text.text);
            break;
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            node_stack.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return title;
}

///
/// getting links from html document
///
QSet<QString> getLinks(QNetworkReply *reply)
{
    QSet<QString> links;
    QStack<GumboNode*> node_stack;
    GumboOutput *output = gumbo_parse(reply->readAll());
    node_stack.push(getBody(output->root));

    GumboNode *node;
    while(!node_stack.isEmpty())
    {
        node = node_stack.pop();
        if (node->type != GUMBO_NODE_ELEMENT)
            continue;

        if (node->v.element.tag == GUMBO_TAG_A){
            GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
            if (href)
                links.insert(QString::fromUtf8(href->value));
            continue;
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            node_stack.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return links;
}
