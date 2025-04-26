# start_conc.py
from kivy.app import App
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label

class StartConcScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        
        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)
        layout.add_widget(Label(text="Start Concentration (%)"))

        for value in ["0", "10", "20", "30", "40", "50", "60", "70", "80", "90", "100"]:
            btn = Button(text=f"{value}%")
            btn.bind(on_press=self.select_start_conc)
            layout.add_widget(btn)

        back_btn = Button(text="Back")
        back_btn.bind(on_press=self.go_back)
        layout.add_widget(back_btn)

        self.add_widget(layout)

    def select_start_conc(self, instance):
        App.get_running_app().start_conc = instance.text  # Save selected start concentration globally
        self.manager.current = 'home'  # Return to home screen

    def go_back(self, instance):
        self.manager.current = 'home'
