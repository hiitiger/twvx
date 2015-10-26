#include "stable.h"
#include "test/test.h"

INT WINAPI WinMain( __in HINSTANCE , __in_opt HINSTANCE , __in_opt LPSTR , __in int  )
{
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    TwApp app;

    TwWindow* w = new TwWindow;
    w->setWindowAttribute(Tw::WA_DeleteOnWindowDestroyed, true);

    TwLabel* label = new TwLabel();
    label->setText(L"See you!");
    label->setFont(TwFont(L"Microsoft YaHei UI", 72));
    label->setBackgroundColor(TwColor(255, 255, 255, 128));
    label->setAcceptClick(true);

    label->setLayout(TwWidget::VBox);

    TwSimpleTextWidget* textWidget = new TwSimpleTextWidget();
    textWidget->setText(L"Hello World!");
    textWidget->setFontSize(72);
    textWidget->setUnderline(true);
    textWidget->setBackgroundColor(TwColor(0xE0, 0XFF, 0XFF, 128));

    TwTextButton* button = new TwTextButton();
    button->setText(L"Hello Button");
    button->setFont(TwFont(L"Microsoft YaHei UI", 72));
    button->setBackgroundColor(TwColor(0XE2E2FF80));

    TwTextButton* button2 = new TwTextButton(button);
    button2->setText(L"Child Button");
    button2->setBackgroundColor(TwColor(0XACACFF80));
    button2->setActiveColor(TwColor(0XACFFAC80));
    button2->setPressedColor(TwColor(0XFFACAC80));

    TwSwitchButton* button3 = new TwSwitchButton;
    button3->setSwitchState(Tw::On);

    TwSlider* progressBar = new TwSlider;


    TwScrollBar* scrollBar = new TwScrollBar;
    scrollBar->setOrientation(Tw::Horizontal);
    scrollBar->setRange(0.f, 50.f);
    scrollBar->setPageStep(10.f);
    scrollBar->setSingleStep(2.f);

    TwImageWidget* imageWidget = new TwImageWidget;
    imageWidget->setImage(TwImage(L"ha.png"));

    label->addWidget(textWidget);
    label->addWidget(button);
    label->addWidget(button3);
    label->addWidget(progressBar);
    label->addWidget(imageWidget);

    label->addWidget(scrollBar);
    w->contentWidget()->addWidget(label);
    w->contentWidget()->setLayout(TwWidget::Fill);
    w->show();

    button2->sigClicked.connect([=](TwObject*)
    {
        button->setEnabled(!button->isEnabled());
        button2->setParentWidget(label);
        button2->setPos(100,150);
        button->setZOrder(10);
        textWidget->setFontSize(12);
    }
    );

    button->sigClicked.connect([=](TwObject*)
    {
        button2->setEnabled(!button2->isEnabled());
        button2->setParentWidget(button);
        button2->setPos(0, 0);
        button->setZOrder(0);
        textWidget->setFontSize(120);
    }
    );

    w->show();
    TwTimer::startTimer(100, [=]()
    {
        button2->setWidth(button2->width() + 1);
        button2->setHeight(button2->height() + 1);
        return true;
    });

//     for (int i = 0; i < 5000; ++i)
//     {
//         TwTextButton* cc = new TwTextButton();
//         cc->setText(L"Hello Button");
//         label->addWidget(cc);
//     }
//    TwTestsR::get()->run();

    app.run();

    return 0;
}