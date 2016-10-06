std::unique_ptr<NotifyIcon> CreateNotifyIcon()
{
    std::unique_ptr<NotifyIcon> icon(new NotifyIcon());
    icon->setText("Blah blah blah");
    icon->setIcon(std::unique_ptr<Icon>(
    	new Icon(GetType(), "cool.ico")
    ));
    icon->setVisible(true);
    return icon;
}
