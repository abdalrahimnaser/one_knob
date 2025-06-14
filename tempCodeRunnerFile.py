w")
        screen_combo.bind('<<ComboboxSelected>>', self.on_screen_change)
        
        # Flash address
        addr_frame = ttk.LabelFrame(main_frame, text="Flash Settings", padding="5")
        addr_frame.pack(fill="x", pady=5)
        
        ttk.Label(addr_frame, text="Flash Address:").grid(row=0, column=0, sticky="w", padx=5)
        self.addr_var = tk.StringVar(value=self.flash_address)
        addr_entry = ttk.Entry(addr_frame, textvariable=self.addr_var, width=10)
        addr_entry.grid(row=0, column=1, padx=5, pady