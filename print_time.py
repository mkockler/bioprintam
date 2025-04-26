# print_time.py
from kivy.app import App
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label

class PrintTimeScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)

        layout.add_widget(Label(text="Print Time (sec)"))

        for value in ["30", "60", "90", "120"]:
            btn = Button(text=f"{value} sec")
            btn.bind(on_press=self.select_print_time)
            layout.add_widget(btn)

        back_btn = Button(text="Back")
        back_btn.bind(on_press=self.go_back)
        layout.add_widget(back_btn)

        self.add_widget(layout)

    def select_print_time(self, instance):
        App.get_running_app().print_time = instance.text  # Save selected print time globally
        self.manager.current = 'home'  # Return to home screen

    def go_back(self, instance):
        self.manager.current = 'home'
