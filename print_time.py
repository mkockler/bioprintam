# print_time.py
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.button import Button

class PrintTimeScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)

        layout.add_widget(Label(text="Print Time (sec)"))
        for value in ["30", "60", "90", "120"]:
            layout.add_widget(Button(text=f"{value} sec"))

        layout.add_widget(Button(text="Back", on_press=self.go_back))
        self.add_widget(layout)

    def go_back(self, instance):
        self.manager.current = 'home'
