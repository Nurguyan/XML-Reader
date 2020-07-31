#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include <QString>
#include <QMetaType>

class TextEditor
{
public:   
    TextEditor();
    explicit TextEditor(QString text_editor, QString file_formats, QString encoding, bool has_intellisense, bool has_plugins, bool can_compile) noexcept;
    ~TextEditor();

    QString getName() const;
    void setName(const QString &value);

    QString getFormats() const;
    void setFormats(const QString &value);

    QString getEncoding() const;
    void setEncoding(const QString &value);

    bool hasIntellisense() const;
    void setIntellisense(bool value);

    bool hasPlugins() const;
    void setPlugins(bool value);

    bool hasCompiler() const;
    void setCompiles(bool value);

    QString toString() const;

private:
    QString name;
    QString formats;
    QString encoding;
    bool intellisense;
    bool plugins;
    bool compiles;
};

Q_DECLARE_METATYPE(TextEditor)

#endif // TEXTEDITOR_H
