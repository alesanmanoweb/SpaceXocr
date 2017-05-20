#ifndef SXOQPLAINTEXT_H
#define SXOQPLAINTEXT_H

#include <QtWidgets>

class SXOQPlainText : public QPlainTextEdit
{
	Q_OBJECT

public:
	SXOQPlainText(QWidget *parent = 0);
	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();

protected:
	virtual void keyPressEvent(QKeyEvent *e);
	void resizeEvent(QResizeEvent *event) override;

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);

private:
	QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget
{
public:
	LineNumberArea(SXOQPlainText *editor) : QWidget(editor)
	{
		codeEditor = editor;
	}

	QSize sizeHint() const override
	{
		return QSize(codeEditor->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) override
	{
		codeEditor->lineNumberAreaPaintEvent(event);
	}

private:
	SXOQPlainText *codeEditor;
};

#endif // SXOQPLAINTEXT_H
