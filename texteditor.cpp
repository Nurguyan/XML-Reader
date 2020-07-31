#include "texteditor.h"

TextEditor::TextEditor(){

}

TextEditor::TextEditor(QString text_editor, QString file_formats, QString encoding, bool has_intellisense, bool has_plugins, bool can_compile) noexcept : name(text_editor), formats(file_formats), encoding(encoding), intellisense(has_intellisense), plugins(has_plugins), compiles(can_compile) {}


TextEditor::~TextEditor()
{

}

QString TextEditor::getName() const
{
    return name;
}

void TextEditor::setName(const QString &value)
{
    name = value;
}

QString TextEditor::getFormats() const
{
    return formats;
}

void TextEditor::setFormats(const QString &value)
{
    formats = value;
}

QString TextEditor::getEncoding() const
{
    return encoding;
}

void TextEditor::setEncoding(const QString &value)
{
    encoding = value;
}

bool TextEditor::hasIntellisense() const
{
    return intellisense;
}

void TextEditor::setIntellisense(bool value)
{
    intellisense = value;
}

bool TextEditor::hasPlugins() const
{
    return plugins;
}

void TextEditor::setPlugins(bool value)
{
    plugins = value;
}

bool TextEditor::hasCompiler() const
{
    return compiles;
}

void TextEditor::setCompiles(bool value)
{
    compiles = value;
}

QString TextEditor::toString() const
{
   return QString("%1, %2, %3, %4, %5, %6").arg(name).arg(formats).arg(encoding).arg(intellisense?"true":"false")
            .arg(plugins?"true":"false").arg(compiles?"true":"false");
}
