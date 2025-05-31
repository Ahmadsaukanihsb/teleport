local Players = game:GetService("Players")
local player = Players.LocalPlayer
local playerGui = player:WaitForChild("PlayerGui")

local gui = playerGui:WaitForChild("GiftNotification", 15)

print("Daftar anak GiftNotification:")
for _, child in ipairs(gui:GetChildren()) do
    print("-", child.Name)
end

-- Cek anak-anak lebih dalam
local frame = gui:FindFirstChild("Frame")
if frame then
    print("Daftar anak Frame:")
    for _, child in ipairs(frame:GetChildren()) do
        print("-", child.Name)
    end

    local giftNotification = frame:FindFirstChild("Gift_Notification")
    if giftNotification then
        print("Daftar anak Gift_Notification:")
        for _, child in ipairs(giftNotification:GetChildren()) do
            print("-", child.Name)
        end

        local holder = giftNotification:FindFirstChild("Holder")
        if holder then
            print("Daftar anak Holder:")
            for _, child in ipairs(holder:GetChildren()) do
                print("-", child.Name)
            end

            local petTextLabel = holder:FindFirstChild("TextLabel")
            local notifUI = holder:FindFirstChild("Notification_UI")
            local notifTextLabel = notifUI and notifUI:FindFirstChild("TextLabel")

            if petTextLabel then
                print("Pet TextLabel Text:", petTextLabel.Text)
            else
                print("TextLabel pet tidak ditemukan")
            end

            if notifTextLabel then
                print("Notification_UI TextLabel Text:", notifTextLabel.Text)
            else
                print("Notification_UI TextLabel tidak ditemukan")
            end
        else
            print("Holder tidak ditemukan")
        end
    else
        print("Gift_Notification tidak ditemukan")
    end
else
    print("Frame tidak ditemukan")
end
