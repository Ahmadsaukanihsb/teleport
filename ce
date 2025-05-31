local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")

local player = Players.LocalPlayer
local AcceptPetGift = ReplicatedStorage:WaitForChild("GameEvents"):WaitForChild("AcceptPetGift")

-- Tunggu GUI Gift muncul
local gui = player:WaitForChild("PlayerGui")
local found = false

-- Fungsi untuk terima gift otomatis
local function checkGiftUI(container)
    for _, frame in ipairs(container:GetChildren()) do
        if frame:IsA("Frame") or frame:IsA("TextButton") then
            local petNameLabel = frame:FindFirstChild("PetName") or frame:FindFirstChild("Pet")
            local senderLabel = frame:FindFirstChild("Sender") or frame:FindFirstChild("From")

            if petNameLabel and senderLabel and petNameLabel:IsA("TextLabel") and senderLabel:IsA("TextLabel") then
                local petName = petNameLabel.Text
                local sender = senderLabel.Text
                print("🎁 Gift ditemukan! Pet:", petName, "| Dari:", sender)

                -- FireServer ke server
                AcceptPetGift:FireServer(petName, sender)
                task.wait(0.5)
                found = true
            end
        end
    end
end

-- Cari folder GiftList di GUI (loop aman)
task.spawn(function()
    while not found do
        for _, descendant in ipairs(gui:GetDescendants()) do
            if descendant:IsA("Frame") and descendant.Name:lower():find("gift") then
                checkGiftUI(descendant)
            end
        end
        task.wait(1)
    end
end)
