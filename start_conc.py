# start_conc.py
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label

class StartConcScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        
        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)
        layout.add_widget(Label(text="Start Concentration (%)"))

        for value in ["0", "25", "50", "75", "100"]:
            layout.add_widget(Button(text=f"{value}"))

        layout.add_widget(Button(text="Back", on_press=self.go_back))
        self.add_widget(layout)

    def go_back(self, instance):
        self.manager.current = 'home'
