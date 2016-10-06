NotifyIcon *CreateNotifyIcon()
{
    NotifyIcon *icon = new NotifyIcon();
    icon->setText("Blah blah blah");
    icon->setIcon(new Icon(GetType(), "cool.ico"));
    icon->setVisible(true);
    return icon;
}
