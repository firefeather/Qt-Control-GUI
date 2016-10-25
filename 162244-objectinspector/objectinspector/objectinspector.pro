TEMPLATE = subdirs

SUBDIRS += objectinspector \
    demo

demo.depends = objectinspector
